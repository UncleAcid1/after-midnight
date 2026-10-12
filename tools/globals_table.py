#!/usr/bin/env python3
"""Write reference/globals_detail.tsv: name, address, section, size, source file, and the
initial bytes (hex, up to 64) for initialized data. Sizes come from the gap to the next
symbol in the same section, so the last symbol of a section may be overestimated."""
import os, sys, lief, bisect
ROOT = os.path.join(os.path.dirname(__file__), "..")
b = lief.MachO.parse(os.path.join(ROOT, "build/re/mm_i386")).at(0)
rows = [l.rstrip("\n").split("\t") for l in open(os.path.join(ROOT, "reference/globals.tsv"))]
sects = {s.name: s for s in b.sections}
# all symbol addresses (functions and data) to bound sizes
addrs = sorted({s.value for s in b.symbols if s.value})
out = []
for name, addr, sect, src in rows:
    a = int(addr, 16)
    if sect not in ("__data", "__common", "__bss", "__const", "__cstring", "__cfstring"):
        continue
    sec = sects[sect]
    i = bisect.bisect_right(addrs, a)
    nxt = addrs[i] if i < len(addrs) else sec.virtual_address + sec.size
    nxt = min(nxt, sec.virtual_address + sec.size)
    size = nxt - a
    init = ""
    if sect in ("__data", "__const", "__cstring", "__cfstring"):
        off = a - sec.virtual_address
        data = bytes(sec.content[off:off + min(size, 64)])
        init = data.hex()
    out.append((name.lstrip("_"), addr, sect, size, src.split("/")[-1], init))
with open(os.path.join(ROOT, "reference/globals_detail.tsv"), "w") as f:
    for r in out:
        f.write("\t".join(str(x) for x in r) + "\n")
print(len(out), "globals")
