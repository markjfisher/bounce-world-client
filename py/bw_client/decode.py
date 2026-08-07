"""Decode bounce-world client data (x-w) payloads."""

from __future__ import annotations

from dataclasses import dataclass


STATUS_FLAGS = (
    (1, "CLIENT_CHANGE"),
    (2, "OBJECT_CHANGE"),
    (4, "FROZEN_TOGGLE"),
    (8, "CLIENT_CMD"),
    (32, "COLLISION"),
)


@dataclass(frozen=True)
class ShapePos:
    shape_id: int
    x: int
    y: int


@dataclass(frozen=True)
class ClientData:
    """Decoded payload from ``x-w <id>`` (header already stripped)."""

    step: int
    status: int
    shapes: tuple[ShapePos, ...]
    raw: bytes
    note: str | None = None

    @property
    def status_names(self) -> list[str]:
        return [name for bit, name in STATUS_FLAGS if self.status & bit]


def decode_client_data(payload: bytes) -> ClientData:
    """
    Decode ``getWorldData`` payload:

    ``[step:u8][status:u8][count:u8][shapeId:u8][x:i8][y:i8]*count``

    Unknown / missing client returns a single ``0`` byte from the server.
    """
    if not payload:
        return ClientData(step=0, status=0, shapes=(), raw=payload, note="empty payload")

    if len(payload) == 1 and payload[0] == 0:
        return ClientData(
            step=0,
            status=0,
            shapes=(),
            raw=payload,
            note="server returned 0 (unknown client or error)",
        )

    if len(payload) < 3:
        return ClientData(
            step=payload[0] if payload else 0,
            status=payload[1] if len(payload) > 1 else 0,
            shapes=(),
            raw=payload,
            note=f"short payload ({len(payload)} bytes); expected at least step/status/count",
        )

    step = payload[0]
    status = payload[1]
    count = payload[2]
    expected = 3 + count * 3
    shapes: list[ShapePos] = []
    note: str | None = None

    if len(payload) < expected:
        note = f"truncated shape list: have {len(payload)} bytes, need {expected} for count={count}"
        available = (len(payload) - 3) // 3
    else:
        available = count
        if len(payload) > expected:
            note = f"trailing {len(payload) - expected} byte(s) after shape list"

    offset = 3
    for _ in range(available):
        shape_id = payload[offset]
        x = int.from_bytes(payload[offset + 1 : offset + 2], "little", signed=True)
        y = int.from_bytes(payload[offset + 2 : offset + 3], "little", signed=True)
        shapes.append(ShapePos(shape_id=shape_id, x=x, y=y))
        offset += 3

    return ClientData(
        step=step,
        status=status,
        shapes=tuple(shapes),
        raw=payload,
        note=note,
    )


def format_client_data(data: ClientData, *, packet_total: int | None = None) -> str:
    """Human-readable explanation of a client-data response."""
    lines: list[str] = []
    if packet_total is not None:
        lines.append(f"framed total size: {packet_total} (header 2 + payload {len(data.raw)})")
    lines.append(f"payload length: {len(data.raw)} bytes")

    if data.note and len(data.raw) <= 1:
        lines.append(data.note)
        return "\n".join(lines)

    status_bits = ", ".join(data.status_names) if data.status_names else "none"
    lines.append(f"step:   {data.step} (0x{data.step:02x})")
    lines.append(f"status: {data.status} (0x{data.status:02x}) [{status_bits}]")
    lines.append(f"shapes: {len(data.shapes)}")
    for i, shape in enumerate(data.shapes):
        lines.append(
            f"  [{i:3d}] id={shape.shape_id:3d}  x={shape.x:4d}  y={shape.y:4d}"
        )
    if data.note:
        lines.append(f"note: {data.note}")
    return "\n".join(lines)


def format_hexdump(data: bytes, *, width: int = 16) -> str:
    """Classic hex+ASCII dump."""
    lines: list[str] = []
    for offset in range(0, len(data), width):
        chunk = data[offset : offset + width]
        hex_part = " ".join(f"{b:02x}" for b in chunk)
        ascii_part = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
        lines.append(f"{offset:04x}  {hex_part:<{width * 3 - 1}}  {ascii_part}")
    return "\n".join(lines) if lines else "(empty)"
