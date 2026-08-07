# bw-client

Command-line helpers for talking to a [bounce-world](https://github.com/markjfisher/bounce-world) server from a normal machine (no FujiNet required).

Useful for checking framed TCP responses while debugging 8-bit clients.

## Run

From the repo root:

```bash
./scripts/bw-client --help
```

The runner sets `PYTHONPATH` to `py/` and invokes `python3 -m bw_client.cli`.

## Defaults

| Setting | Default |
| --- | --- |
| Host | `localhost` |
| HTTP (REST) port | `8080` |
| Framed TCP port | `9003` |
| New client version | `2` |
| New client screen size | `40×80` |

## Examples

### Create a client

HTTP `POST /client` — returns the assigned client id:

```bash
./scripts/bw-client add-client --name coco
# client id: 1
```

Override size / version / host:

```bash
./scripts/bw-client add-client --name dos \
  --width 80 --height 24 --version-num 2 \
  --world-width 80 --world-height 24 \
  --host 192.168.1.10
```

### Fetch client world data

Sends framed TCP `x-w <id>`, reads the length-prefixed response, then closes (no 30s idle wait):

```bash
./scripts/bw-client fetch 1
```

Example decoded output:

```text
framed total size: 5 (header 2 + payload 3)
payload length: 3 bytes
step:   251 (0xfb)
status: 1 (0x01) [CLIENT_CHANGE]
shapes: 0
```

When shapes are present, each entry is `id / x / y` (signed screen coordinates).

### Hex dump instead of decode

Full framed packet (2-byte LE size + payload):

```bash
./scripts/bw-client fetch 1 --hexdump
```

```text
0000  05 00 fb 01 00                                   .....
```

Payload only:

```bash
./scripts/bw-client fetch 1 --hexdump --payload-only
```

### Arbitrary TCP command

```bash
./scripts/bw-client raw "ws" --hexdump
./scripts/bw-client raw "shape-count"
./scripts/bw-client raw "who"
```

Commands are sent with an `x-` prefix if missing (same as persistent clients). The tool still closes after one framed reply.

## Payload layout (`fetch` / `x-w`)

After the 2-byte little-endian total size header:

| Offset | Field | Notes |
| --- | --- | --- |
| 0 | `step` | Simulator step byte |
| 1 | `status` | Bit flags (see below) |
| 2 | `count` | Number of shapes (0–240) |
| 3… | shapes | `shapeId`, `x`, `y` per shape (1 byte each; `x`/`y` signed) |

Status flags:

| Bit | Name |
| --- | --- |
| 1 | `CLIENT_CHANGE` |
| 2 | `OBJECT_CHANGE` |
| 4 | `FROZEN_TOGGLE` |
| 8 | `CLIENT_CMD` |
| 32 | `COLLISION` |

A single payload byte `00` means unknown client / error.
