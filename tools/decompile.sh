#!/usr/bin/env bash
# Regenerate reference/decomp from the original executable using Ghidra headless.
# Usage: GHIDRA=/opt/ghidra_11.4.2_PUBLIC tools/decompile.sh
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
GHIDRA="${GHIDRA:-/opt/ghidra_11.4.2_PUBLIC}"
WORK="$ROOT/build/re"
mkdir -p "$WORK/project"
python3 "$ROOT/tools/extract_symbols.py" "$WORK"
python3 "$ROOT/tools/gen_swx_prototypes.py"
cp "$WORK/filemap.tsv" "$WORK/globals.tsv" "$ROOT/reference/"
if [ ! -d "$WORK/project/MM.rep" ]; then
  "$GHIDRA/support/analyzeHeadless" "$WORK/project" MM -import "$WORK/mm_i386" \
    -processor x86:LE:32:default -cspec gcc
fi
rm -rf "$ROOT/reference/decomp"
"$GHIDRA/support/analyzeHeadless" "$WORK/project" MM -process mm_i386 -noanalysis \
  -scriptPath "$ROOT/tools/ghidra" \
  -postScript ApplyTypes.java "$ROOT/tools/ghidra/types.txt" "$ROOT/tools/ghidra/prototypes_swx.h" "$ROOT/tools/ghidra/prototypes.h" \
  -postScript ExportByFile.java \
  "$WORK/filemap.tsv" "$ROOT/reference/decomp"
