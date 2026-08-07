"""bw-client CLI."""

from __future__ import annotations

import argparse
import sys

from . import __version__
from .decode import decode_client_data, format_client_data, format_hexdump
from .http_api import add_client
from .tcp import send_command


def _add_host_args(parser: argparse.ArgumentParser) -> None:
    parser.add_argument(
        "--host",
        default="localhost",
        help="server hostname (default: localhost)",
    )


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="bw-client",
        description="Send bounce-world client requests from the command line.",
    )
    parser.add_argument("--version", action="version", version=f"%(prog)s {__version__}")
    sub = parser.add_subparsers(dest="cmd", required=True)

    add = sub.add_parser(
        "add-client",
        help="create a client via HTTP POST /client (port 8080 by default)",
    )
    _add_host_args(add)
    add.add_argument("--http-port", type=int, default=8080, help="REST port (default: 8080)")
    add.add_argument("--name", required=True, help="client name")
    add.add_argument("--version-num", type=int, default=2, dest="client_version",
                     help="client protocol version (default: 2)")
    add.add_argument("--width", type=int, default=40, help="screen width (default: 40)")
    add.add_argument("--height", type=int, default=24, help="screen height (default: 24)")
    add.add_argument("--world-width", type=int, default=None, help="optional world region width")
    add.add_argument("--world-height", type=int, default=None, help="optional world region height")
    add.add_argument("--timeout", type=float, default=5.0, help="socket timeout seconds")
    add.set_defaults(fn=cmd_add_client)

    fetch = sub.add_parser(
        "fetch",
        help="fetch client world data via framed TCP (x-w <id>, port 9003 by default)",
    )
    _add_host_args(fetch)
    fetch.add_argument("--tcp-port", type=int, default=9003, help="framed TCP port (default: 9003)")
    fetch.add_argument("client_id", type=int, help="client id to query")
    fetch.add_argument(
        "--hexdump",
        action="store_true",
        help="print the framed packet as a hex dump instead of decoded context",
    )
    fetch.add_argument(
        "--payload-only",
        action="store_true",
        help="with --hexdump, dump payload only (omit 2-byte size header)",
    )
    fetch.add_argument("--timeout", type=float, default=5.0, help="socket timeout seconds")
    fetch.set_defaults(fn=cmd_fetch)

    raw = sub.add_parser(
        "raw",
        help="send an arbitrary framed TCP command and dump the response",
    )
    _add_host_args(raw)
    raw.add_argument("--tcp-port", type=int, default=9003, help="framed TCP port (default: 9003)")
    raw.add_argument("command", help='command text, e.g. "w 1" or "ws" (x- added if missing)')
    raw.add_argument("--hexdump", action="store_true", help="print hex dump of the framed packet")
    raw.add_argument("--timeout", type=float, default=5.0, help="socket timeout seconds")
    raw.set_defaults(fn=cmd_raw)

    return parser


def cmd_add_client(args: argparse.Namespace) -> int:
    client_id = add_client(
        args.host,
        args.http_port,
        name=args.name,
        version=args.client_version,
        width=args.width,
        height=args.height,
        world_width=args.world_width,
        world_height=args.world_height,
        timeout=args.timeout,
    )
    print(f"client id: {client_id}")
    return 0


def cmd_fetch(args: argparse.Namespace) -> int:
    response = send_command(
        args.host,
        args.tcp_port,
        f"w {args.client_id}",
        timeout=args.timeout,
    )
    if args.hexdump:
        data = response.payload if args.payload_only else response.packet
        print(format_hexdump(data))
        return 0

    decoded = decode_client_data(response.payload)
    print(format_client_data(decoded, packet_total=response.total_size))
    return 0


def cmd_raw(args: argparse.Namespace) -> int:
    response = send_command(
        args.host,
        args.tcp_port,
        args.command,
        timeout=args.timeout,
    )
    if args.hexdump:
        print(format_hexdump(response.packet))
    else:
        print(
            f"framed total size: {response.total_size}\n"
            f"payload ({len(response.payload)} bytes):\n"
            f"{format_hexdump(response.payload)}"
        )
    return 0


def main(argv: list[str] | None = None) -> None:
    parser = build_parser()
    args = parser.parse_args(argv)
    try:
        raise SystemExit(args.fn(args))
    except (OSError, RuntimeError, ValueError, ConnectionError) as exc:
        print(f"bw-client: {exc}", file=sys.stderr)
        raise SystemExit(1) from exc


if __name__ == "__main__":
    main()
