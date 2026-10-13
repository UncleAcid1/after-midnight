#!/usr/bin/env python3
"""Packs each mansion folder into one file, for the PS3 package.

A mansion is a folder of small files (one per room and layer): the game's mansions come to
about 120,000 files, which the PS3 installs very slowly. Each folder given here (a folder of
mansions) has its mansions' contents replaced by a single file "Mansion.pack", which
src/ps3/PS3Files.c reads as if the files were still there.

    pack_mansions.py <folder of mansions>...

The format (numbers big-endian):
    "AMPK", version 1, number of files, size of the names
    per file, sorted by name: offset of its name, offset of its data, its size
    the names (paths inside the mansion, "/" between folders), each ended by a zero byte
    the files' data
"""
import os
import shutil
import struct
import sys

PACK_NAME = "Mansion.pack"


def pack_folder(folder):
    files = []
    for root, dirs, names in os.walk(folder):
        dirs.sort()
        for name in names:
            path = os.path.join(root, name)
            rel = os.path.relpath(path, folder).replace(os.sep, "/")
            if rel != PACK_NAME:
                files.append((rel.encode("utf-8"), path))
    files.sort()
    names = bytearray()
    name_offsets = []
    for rel, _ in files:
        name_offsets.append(len(names))
        names += rel + b"\0"
    header_size = 16 + 12 * len(files) + len(names)
    entries = bytearray()
    data_offset = header_size
    sizes = []
    for i, (_, path) in enumerate(files):
        size = os.path.getsize(path)
        entries += struct.pack(">III", name_offsets[i], data_offset, size)
        sizes.append(size)
        data_offset += size
    tmp = os.path.join(folder, PACK_NAME + ".tmp")
    with open(tmp, "wb") as out:
        out.write(b"AMPK" + struct.pack(">III", 1, len(files), len(names)))
        out.write(entries)
        out.write(names)
        for _, path in files:
            with open(path, "rb") as f:
                out.write(f.read())
    for entry in os.listdir(folder):
        path = os.path.join(folder, entry)
        if entry == PACK_NAME + ".tmp":
            continue
        if os.path.isdir(path) and not os.path.islink(path):
            shutil.rmtree(path)
        else:
            os.remove(path)
    os.rename(tmp, os.path.join(folder, PACK_NAME))
    return len(files)


def main():
    total = folders = 0
    for parent in sys.argv[1:]:
        for name in sorted(os.listdir(parent)):
            path = os.path.join(parent, name)
            if os.path.isdir(path) and not name.startswith("."):
                total += pack_folder(path)
                folders += 1
    print("packed %d files into %d mansions" % (total, folders))


if __name__ == "__main__":
    main()
