#!/usr/bin/env python3
"""Print decompiled functions from reference/decomp, compacted for reading.

Usage: tools/show.py <decomp file substring> [function name regex] [--max-bytes N]
Assertion boilerplate is collapsed to ASSERT(line) and blank lines removed.
"""
import re, sys, glob, os
ROOT = os.path.join(os.path.dirname(__file__), "..", "reference", "decomp")
args = [a for a in sys.argv[1:] if not a.startswith("--")]
maxb = int(sys.argv[sys.argv.index("--max-bytes") + 1]) if "--max-bytes" in sys.argv else 10**9
if "--max-bytes" in sys.argv:
    args.remove(str(maxb))
files = [f for f in glob.glob(ROOT + "/**/*", recursive=True) if os.path.isfile(f) and args[0] in f]
pat = re.compile(args[1]) if len(args) > 1 else None

# Resolve PTR_LAB_<addr> (pointers to functions Ghidra left unnamed) through the
# binary's pointer table and the function map.
_funcs = {}
for line in open(os.path.join(ROOT, "..", "filemap.tsv")):
    parts = line.rstrip("\n").split("\t")
    if len(parts) >= 3:
        _funcs[int(parts[2], 16)] = parts[1].lstrip("_")
_binary = None
def resolve_ptr(m):
    global _binary
    addr = int(m.group(1), 16)
    try:
        if _binary is None:
            import lief
            _binary = lief.MachO.parse(os.path.join(ROOT, "..", "..", "build", "re", "mm_i386")).at(0)
        for sec in _binary.sections:
            if sec.virtual_address <= addr < sec.virtual_address + sec.size:
                off = addr - sec.virtual_address
                target = int.from_bytes(bytes(sec.content)[off:off + 4], "little")
                return "&" + _funcs.get(target, "LAB_%x" % target)
    except Exception:
        pass
    return m.group(0)

# Game sprite records extend SpriteRec past 0xf8. Ghidra shows those bytes as f_XXX
# (from the padded SpriteRec in types.txt) or as *(T *)((int)p + 0xXXX). Collapse both
# into p->_XXX with a type prefix: b_ byte, s_ short, l_ 32-bit int, fl_ float, p_ pointer,
# w_/d_ 2/4 bytes assembled byte-by-byte (type unknown).
_TP = {"short": "s", "ushort": "s", "undefined2": "s", "int": "l", "uint": "l", "long": "l",
       "undefined4": "l", "float": "fl", "char": "b", "uchar": "b", "undefined1": "b",
       "undefined": "b", "bool": "b", "SWBoolean": "b", "Boolean": "b", "undefined8": "q",
       "double": "q"}
def _tp(t):
    t = t.strip()
    if t.endswith("*"):
        return "p"
    return _TP.get(t, "t" + t + "_")
def collapse_fields(b):
    # x._0_1_ = p->f_A; x._1_1_ = p->f_B; ...  ->  x = p->w_A;
    def splice_read(m):
        lines = m.group(0)
        parts = re.findall(r"(\w+)\._(\d)_1_ = (\w+)->f_([0-9a-f]{3});", lines)
        n = len(parts)
        pre = "w" if n == 2 else "d"
        return "%s%s = %s->%s_%s;" % (m.group(1), parts[0][0], parts[0][2], pre, parts[0][3])
    b = re.sub(r"(\s*)(\w+)\._0_1_ = (\w+)->f_\w{3};(?:\s*\2\._[1-3]_1_ = \3->f_\w{3};)+",
               lambda m: splice_read(m).replace(m.group(1) + m.group(1), m.group(1)), b)
    # p->f_A = (char)x; p->f_B = (char)((ushort)x >> 8);  ->  p->w_A = x;
    b = re.sub(r"(\w+)->f_(\w{3}) = \(char\)(\w+);\s*\1->f_\w{3} = \(char\)\(\(u?(?:short|int)\)\3 >> 8\);"
               r"(?:\s*\1->f_\w{3} = \(char\)\(\(uint\)\3 >> 0x10\);\s*\1->f_\w{3} = \(char\)\(\(uint\)\3 >> 0x18\);)?",
               r"\1->w_\2 = \3;", b)
    # consecutive zero-byte stores: p->f_A = '\0'; p->f_A+1 = '\0'; ...  ->  p->zN_A = 0;
    def zeros(m):
        offs = [int(o, 16) for o in re.findall(r"->f_(\w{3}) = '\\0';", m.group(0))]
        runs, out = [[offs[0]]], []
        for o in offs[1:]:
            if o == runs[-1][-1] + 1:
                runs[-1].append(o)
            else:
                runs.append([o])
        for r in runs:
            if len(r) == 1:
                out.append("%s->b_%03x = 0;" % (m.group(1), r[0]))
            else:
                out.append("%s->z%d_%03x = 0;" % (m.group(1), len(r), r[0]))
        return " ".join(out)
    b = re.sub(r"(\w+)->f_\w{3} = '\\0';(?:\s*\1->f_\w{3} = '\\0';)+", zeros, b)
    # *(T *)&p->f_A  ->  p->T_A
    b = re.sub(r"\*\((\w+ ?\**)\s*\*\)&(\w+)->f_(\w{3})", lambda m: "%s->%s_%s" % (m.group(2), _tp(m.group(1)), m.group(3)), b)
    b = re.sub(r"&(\w+)->f_(\w{3})", r"&\1->_\2", b)
    b = re.sub(r"(\w+)->f_(\w{3})", r"\1->b_\2", b)
    # *(T *)((int)p + 0xA)  ->  p->T_A ; (int)p + 0xA -> &p->_A
    b = re.sub(r"\*\((\w+ ?\**)\s*\*\)\s*\(\(int\)(\w+) \+ 0x(1[0-9a-f]{2}|f[89a-f])\)",
               lambda m: "%s->%s_%03x" % (m.group(2), _tp(m.group(1)), int(m.group(3), 16)), b)
    b = re.sub(r"\(int\)(\w+) \+ 0x(1[0-9a-f]{2}|f[89a-f])\b", lambda m: "&%s->_%03x" % (m.group(1), int(m.group(2), 16)), b)
    # same with an int-typed sprite parameter: *(T *)(p + 0xA) and p + 0xA
    b = re.sub(r"\*\((\w+ ?\**)\s*\*\)\s*\((param_\d+|\w*[sS]prite\w*) \+ 0x(1[0-9a-f]{2}|f[89a-f])\)",
               lambda m: "%s->%s_%03x" % (m.group(2), _tp(m.group(1)), int(m.group(3), 16)), b)
    b = re.sub(r"\b(param_\d+) \+ 0x(1[0-9a-f]{2}|f[89a-f])\b", lambda m: "&%s->_%03x" % (m.group(1), int(m.group(2), 16)), b)
    return b

for fn in files:
    src = open(fn).read()
    for f in src.split("\n// ---- ")[1:]:
        name = f.split(" @")[0]
        size = int(re.search(r"\((\d+) bytes\)", f.split("\n", 1)[0]).group(1))
        if pat and not pat.search(name):
            continue
        if size > maxb:
            print(f"// {name} ({size} bytes) -- skipped, too large\n")
            continue
        b = re.sub(r"\(\*\*\(code \*\*\)PTR__gSWAssertFailProc_[0-9a-f]+\)\s*\(\"[^\"]*\",\s*PTR____FUNCTION___\w+,\s*(\w+)\);", r"ASSERT(\1);", f, flags=re.S)
        b = re.sub(r"_MMFatalErrorCallback\s*\(([^,]*),\s*\"[^\"]*\",\s*PTR____FUNCTION___\w+,\s*(\w+)\);", r"FATAL(\1, \2);", b, flags=re.S)
        b = re.sub(r"PTR_LAB_00([0-9a-f]{6})", resolve_ptr, b)
        b = re.sub(r"PTR__(\w+?)_001c[0-9a-f]{4}", r"&\1", b)
        b = re.sub(r"\*&", "", b)
        b = collapse_fields(b)
        b = re.sub(r"\n\s*\n", "\n", b)
        print("// " + b.strip() + "\n")
