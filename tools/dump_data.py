#!/usr/bin/env python3
"""Print initialized data from the i386 binary as C initializer values.

Usage: tools/dump_data.py <hex address> <count> <type: f32|f64|i8|u8|i16|u16|i32|u32|str>
Floats print as exact hexadecimal float literals."""
import sys, struct, lief, os
b = lief.MachO.parse(os.path.join(os.path.dirname(__file__), "../build/re/mm_i386")).at(0)
addr, count, typ = int(sys.argv[1], 16), int(sys.argv[2]), sys.argv[3]
fmt = {"f32": "f", "f64": "d", "i8": "b", "u8": "B", "i16": "h", "u16": "H", "i32": "i", "u32": "I"}
for s in b.sections:
    if s.virtual_address <= addr < s.virtual_address + s.size:
        data = bytes(s.content)[addr - s.virtual_address:]
        break
else:
    sys.exit("address not in a section with content")
if typ == "str":
    print(repr(data[:data.index(0)].decode("latin1")))
    sys.exit()
size = struct.calcsize(fmt[typ])
vals = struct.unpack("<%d%s" % (count, fmt[typ]), data[:count * size])
if typ.startswith("f"):
    out = [float(v).hex() + ("f" if typ == "f32" else "") for v in vals]
else:
    out = [str(v) for v in vals]
print(", ".join(out))
