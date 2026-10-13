#!/usr/bin/env python3
"""Gives an ATRAC3plus RIFF file from atracdenc the header Sony's at3tool writes.

The XMB reads the codec settings Sony's tool puts after the format's GUID (12 bytes: a
version, then the channel layout and the frame size), which atracdenc leaves out, and an
8-byte 'fact' chunk (samples, encoder delay).

    fix_at3plus_header.py in.at3 out.at3
"""
import struct
import sys

data = open(sys.argv[1], "rb").read()
assert data[:4] == b"RIFF" and data[8:12] == b"WAVE"
chunks, i = [], 12
while i + 8 <= len(data):
    cid, size = data[i:i + 4], struct.unpack("<I", data[i + 4:i + 8])[0]
    chunks.append([cid, data[i + 8:i + 8 + size]])
    i += 8 + size + (size & 1)
out = []
for cid, body in chunks:
    if cid == b"fmt ":
        tag, channels, rate, avg, align, bits = struct.unpack("<HHIIHH", body[:16])
        assert tag == 0xFFFE, "not ATRAC3plus"
        guid = body[24:40]
        frame = (align // 8 - 1) & 0x3FF
        layout = 1 if channels == 1 else 2
        config = (1 << 13) | (layout << 10) | frame     # 44.1 kHz, channels, frame size
        body = struct.pack("<HHIIHHHHI", tag, channels, rate, avg, align, 0, 34, 2048,
                           3 if channels == 2 else 4) + guid + struct.pack(">HH", 1, config) + bytes(8)
    elif cid == b"fact":
        samples = struct.unpack("<I", body[:4])[0]
        body = struct.pack("<II", samples, 2048 + 184)
    out.append(cid + struct.pack("<I", len(body)) + body + (b"\0" if len(body) & 1 else b""))
riff = b"WAVE" + b"".join(out)
open(sys.argv[2], "wb").write(b"RIFF" + struct.pack("<I", len(riff)) + riff)
