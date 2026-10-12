#!/usr/bin/env python3
"""Compares the width of each global's declaration in src/ with how the original code
accesses it (from reference/decomp). Prints mismatches."""
import re, glob, os, collections
ROOT = os.path.join(os.path.dirname(__file__), "..")
code = ""
for f in glob.glob(ROOT + "/reference/decomp/Sources*/*.c") + glob.glob(ROOT + "/reference/decomp/Sources*/**/*.c", recursive=True):
    code += open(f, errors="ignore").read()
widths = {"char": 1, "undefined1": 1, "byte": 1, "uchar": 1, "bool": 1,
          "short": 2, "undefined2": 2, "ushort": 2,
          "int": 4, "undefined4": 4, "uint": 4, "float": 4, "long": 4}
access = collections.defaultdict(collections.Counter)
for m in re.finditer(r"\*\((\w+) \*\)(?:PTR__|&)(g\w+?)(?:_001c[0-9a-f]+)?\b", code):
    t, name = m.group(1), m.group(2)
    if t in widths:
        access[name][widths[t]] += 1
decl = {}
srcdecl = re.compile(r"^(?:extern\s+|static\s+)?(Boolean|short|long|int|float|char|SWBoolean|Uint8|Sint16|Uint16|Sint32|Uint32|unsigned short|unsigned char|double)\s+([^;(]*);", re.M)
sizes = {"Boolean": 1, "SWBoolean": 1, "char": 1, "Uint8": 1, "unsigned char": 1, "short": 2, "Sint16": 2,
         "Uint16": 2, "unsigned short": 2, "long": 4, "int": 4, "float": 4, "Sint32": 4, "Uint32": 4, "double": 8}
for f in glob.glob(ROOT + "/src/game/*.[ch]"):
    for m in srcdecl.finditer(open(f, errors="ignore").read()):
        for part in m.group(2).split(","):
            part = part.strip()
            nm = re.match(r"\**\s*(g\w+)\s*(\[|=|$)", part)
            if nm and nm.group(2) != "[":
                decl[nm.group(1)] = (m.group(1), os.path.basename(f))
for name, counts in sorted(access.items()):
    if name not in decl:
        continue
    w = counts.most_common(1)[0][0]
    t, f = decl[name]
    if sizes[t] != w:
        print("%-40s declared %-8s (%d) in %-20s accessed as %d bytes %s" % (name, t, sizes[t], f, w, dict(counts)))
