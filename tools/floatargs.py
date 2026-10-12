#!/usr/bin/env python3
"""Recovers float arguments that Ghidra shows as 0 (loaded from the constant pool).

Usage: tools/floatargs.py <text.asm from objdump> <function name> [...]
For each call to the named functions, prints the caller and the stack arguments
(immediates, or float constants read through the PIC base register)."""
import re, sys, struct, os, bisect
ROOT = os.path.join(os.path.dirname(__file__), "..")
sys.path.insert(0, os.path.join(ROOT, "tools"))

funcs = []
for line in open(os.path.join(ROOT, "reference", "filemap.tsv")):
    p = line.rstrip("\n").split("\t")
    if len(p) >= 3:
        funcs.append((int(p[2], 16), p[1].lstrip("_"), p[0]))
funcs.sort()
addrs = [f[0] for f in funcs]
byname = {f[1]: f[0] for f in funcs}

import lief
binary = lief.MachO.parse(os.path.join(ROOT, "build", "re", "mm_i386")).at(0)
def read32(addr):
    for sec in binary.sections:
        if sec.virtual_address <= addr < sec.virtual_address + sec.size:
            off = addr - sec.virtual_address
            return bytes(sec.content)[off:off + 4]
    return None

def owner(addr):
    i = bisect.bisect_right(addrs, addr) - 1
    return funcs[i] if i >= 0 else (0, "?", "?")

lines = [l for l in open(sys.argv[1]) if re.match(r"\s+[0-9a-f]+:", l)]
parsed = []
for l in lines:
    m = re.match(r"\s+([0-9a-f]+):\s+(?:[0-9a-f]{2} )+\s*(.*)", l)
    if m:
        parsed.append((int(m.group(1), 16), m.group(2).strip()))

targets = {byname[n]: n for n in sys.argv[2:] if n in byname}
for idx, (addr, ins) in enumerate(parsed):
    m = re.match(r"call\s+0x([0-9a-f]+)", ins)
    if not m or int(m.group(1), 16) not in targets:
        continue
    fstart, fname, ffile = owner(addr)
    # PIC base: "call X; pop %ebx" at function start -> ebx = X
    ebx = None
    j = idx
    while j > 0 and parsed[j][0] >= fstart:
        mm = re.match(r"pop\s+%ebx", parsed[j][1])
        if mm and re.match(r"call\s+0x", parsed[j - 1][1]):
            ebx = parsed[j][0]
        j -= 1
    args = {}
    regs = {}
    k = idx - 1
    seen = 0
    while k > 0 and seen < 40 and parsed[k][0] >= fstart:
        ins2 = parsed[k][1]
        mm = re.match(r"(?:movl|mov)\s+(\S+),(0x[0-9a-f]+)?\(%esp\)$", ins2)
        if mm:
            off = int(mm.group(2), 16) if mm.group(2) else 0
            if off not in args:
                src = mm.group(1)
                val = src
                if src.startswith("$"):
                    v = int(src[1:], 16) & 0xffffffff
                    f = struct.unpack("<f", struct.pack("<I", v))[0]
                    val = "%d (%g)" % (v if v < 0x80000000 else v - 0x100000000, f)
                elif src.startswith("%"):
                    # look back for "mov (%eax),%eax" after "lea X(%ebx),%eax"
                    for q in range(k - 1, max(k - 6, 0), -1):
                        m2 = re.match(r"mov\s+\(%(\w+)\),%" + src[1:] + "$", parsed[q][1])
                        if m2:
                            for r in range(q - 1, max(q - 4, 0), -1):
                                m3 = re.match(r"lea\s+(0x[0-9a-f]+)\(%ebx\),%" + m2.group(1) + "$", parsed[r][1])
                                if m3 and ebx is not None:
                                    a = ebx + int(m3.group(1), 16)
                                    b = read32(a)
                                    if b:
                                        val = "%g @%x" % (struct.unpack("<f", b)[0], a)
                                    break
                            break
                args[off] = val
        if re.match(r"call\s", ins2):
            break
        k -= 1
        seen += 1
    print("%x %-34s %s(%s)" % (addr, fname, targets[int(m.group(1), 16)],
          ", ".join(str(args.get(o, "?")) for o in sorted(args) if o > 0)))
