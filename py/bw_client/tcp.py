"""Framed TCP helpers for the bounce-world server."""

from __future__ import annotations

import socket
from dataclasses import dataclass


@dataclass(frozen=True)
class FramedResponse:
    """A length-prefixed TCP response."""

    total_size: int
    payload: bytes

    @property
    def packet(self) -> bytes:
        """Full on-wire packet including the 2-byte LE size header."""
        return bytes((self.total_size & 0xFF, (self.total_size >> 8) & 0xFF)) + self.payload


def _recv_exact(sock: socket.socket, nbytes: int) -> bytes:
    chunks: list[bytes] = []
    remaining = nbytes
    while remaining > 0:
        chunk = sock.recv(remaining)
        if not chunk:
            raise ConnectionError(
                f"connection closed after {nbytes - remaining} of {nbytes} bytes"
            )
        chunks.append(chunk)
        remaining -= len(chunk)
    return b"".join(chunks)


def send_command(
    host: str,
    port: int,
    command: str,
    *,
    persistent: bool = True,
    timeout: float = 5.0,
) -> FramedResponse:
    """
    Send one line command and read one framed response, then close.

    Persistent clients prefix commands with ``x-`` (keeps the server connection
    open). This helper always closes after the response, so either form works;
    ``persistent=True`` matches real clients.
    """
    line = command if command.endswith("\n") else f"{command}\n"
    if persistent and not line.startswith("x-"):
        line = f"x-{line}"

    with socket.create_connection((host, port), timeout=timeout) as sock:
        sock.settimeout(timeout)
        sock.sendall(line.encode("utf-8"))
        header = _recv_exact(sock, 2)
        total_size = header[0] | (header[1] << 8)
        if total_size < 2:
            raise ValueError(f"invalid framed packet size: {total_size}")
        payload = _recv_exact(sock, total_size - 2)
        return FramedResponse(total_size=total_size, payload=payload)
