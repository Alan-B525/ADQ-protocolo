#!/usr/bin/env python3
"""Simple frame parser for ADQ protocol captures.

Usage:
    python parse_frames.py HEXSTRING
"""

from __future__ import annotations

import binascii
import struct
import sys


def crc16_ccitt(data: bytes) -> int:
    crc = 0xFFFF
    for byte in data:
        crc ^= byte << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = ((crc << 1) ^ 0x1021) & 0xFFFF
            else:
                crc = (crc << 1) & 0xFFFF
    return crc


def parse_frame(raw: bytes) -> dict:
    if len(raw) < 20:
        raise ValueError("frame too short")

    start, version, msg_type, flags = struct.unpack_from("<BBBB", raw, 0)
    node_id, seq = struct.unpack_from("<HH", raw, 4)
    timestamp_us = struct.unpack_from("<Q", raw, 8)[0]
    payload_len = struct.unpack_from("<H", raw, 16)[0]

    expected_len = 18 + payload_len + 2
    if len(raw) != expected_len:
        raise ValueError(f"invalid len {len(raw)} != {expected_len}")

    payload = raw[18 : 18 + payload_len]
    crc_recv = struct.unpack_from("<H", raw, 18 + payload_len)[0]
    crc_calc = crc16_ccitt(raw[: 18 + payload_len])

    return {
        "start": start,
        "version": version,
        "msg_type": msg_type,
        "flags": flags,
        "node_id": node_id,
        "seq": seq,
        "timestamp_us": timestamp_us,
        "payload_len": payload_len,
        "payload_hex": payload.hex(),
        "crc_recv": crc_recv,
        "crc_calc": crc_calc,
        "crc_ok": crc_recv == crc_calc,
    }


def main() -> int:
    if len(sys.argv) != 2:
        print("usage: python parse_frames.py HEXSTRING")
        return 1

    try:
        raw = binascii.unhexlify(sys.argv[1].strip())
        parsed = parse_frame(raw)
    except Exception as exc:  # pragma: no cover
        print(f"error: {exc}")
        return 1

    for k, v in parsed.items():
        print(f"{k}: {v}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
