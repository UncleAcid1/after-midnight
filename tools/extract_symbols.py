#!/usr/bin/env python3
"""Extract the i386 slice of the original executable and dump its symbol info.

Writes into the given output directory:
  mm_i386        thin i386 Mach-O (input for Ghidra)
  filemap.tsv    original source file -> function name, address, size (from STABS debug map)
  globals.tsv    global/static data symbols: name, address, section, source file

Usage: tools/extract_symbols.py <out_dir>
Requires: pip install lief
"""
import os, sys, lief

EXE = os.path.join(os.path.dirname(__file__), "..", "original",
                   "Midnight Mansion HD.app", "Contents", "MacOS", "Midnight Mansion")

N_GSYM, N_FUN, N_STSYM, N_SO = 0x20, 0x24, 0x26, 0x64

def main(out):
    os.makedirs(out, exist_ok=True)
    binary = lief.MachO.parse(EXE).at(0)  # i386 slice
    binary.write(os.path.join(out, "mm_i386"))
    sects = [(s.virtual_address, s.virtual_address + s.size, s.name) for s in binary.sections]
    def sect(addr):
        for lo, hi, n in sects:
            if lo <= addr < hi:
                return n
        return "?"
    cur, pend = None, None
    funcs, data = [], []
    for s in binary.symbols:
        t = s.raw_type
        if t == N_SO:
            if s.name:
                cur = s.name.split("/Simple - Midnight Mansion/")[-1]
        elif t == N_FUN:
            if s.name:
                pend = (s.name.split(":")[0], s.value)
            elif pend:
                funcs.append((cur, pend[0], pend[1], s.value))
                pend = None
        elif t in (N_GSYM, N_STSYM):
            data.append((s.name.split(":")[0], s.value, cur))
    # N_GSYM entries carry no address in Apple's debug map; take it from the symbol table.
    addr_of = {s.name: s.value for s in binary.symbols if s.raw_type & 0xe0 == 0 and s.raw_type & 0x0e == 0x0e}
    with open(os.path.join(out, "filemap.tsv"), "w") as f:
        for src, name, addr, size in funcs:
            f.write(f"{src}\t{name}\t{addr:x}\t{size}\n")
    with open(os.path.join(out, "globals.tsv"), "w") as f:
        for name, addr, src in data:
            a = addr or addr_of.get(name, 0)
            f.write(f"{name}\t{a:x}\t{sect(a)}\t{src}\n")
    print(f"{len(funcs)} functions, {len(data)} data symbols")

if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else "build/re")
