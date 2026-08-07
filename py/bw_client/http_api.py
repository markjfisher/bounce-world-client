"""HTTP helpers for bounce-world REST endpoints."""

from __future__ import annotations

from urllib.error import HTTPError, URLError
from urllib.request import Request, urlopen


def add_client(
    host: str,
    http_port: int,
    *,
    name: str,
    version: int = 2,
    width: int = 40,
    height: int = 80,
    world_width: int | None = None,
    world_height: int | None = None,
    timeout: float = 5.0,
) -> int:
    """
    POST /client and return the assigned client id.

    Body format: ``name,version,width,height[,worldWidth,worldHeight]``
    """
    parts = [name, str(version), str(width), str(height)]
    if world_width is not None or world_height is not None:
        if world_width is None or world_height is None:
            raise ValueError("world_width and world_height must both be set")
        parts.extend([str(world_width), str(world_height)])

    body = ",".join(parts).encode("utf-8")
    url = f"http://{host}:{http_port}/client"
    request = Request(url, data=body, method="POST")
    request.add_header("Content-Type", "text/plain")

    try:
        with urlopen(request, timeout=timeout) as response:
            data = response.read()
    except HTTPError as exc:
        detail = exc.read().decode("utf-8", errors="replace")
        raise RuntimeError(f"add-client failed ({exc.code}): {detail}") from exc
    except URLError as exc:
        raise RuntimeError(f"add-client failed: {exc.reason}") from exc

    if len(data) != 1:
        raise RuntimeError(f"unexpected add-client response: {data!r}")
    client_id = data[0]
    if client_id == 0:
        raise RuntimeError("server returned client id 0 (error)")
    return client_id
