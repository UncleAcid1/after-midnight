#!/usr/bin/env python3
"""Writes src/game/MMProcTable.h: every sprite move and frame proc the game declares in
MMGameProtos.h, in a fixed order, so a sprite's procs can be named by number between
computers (MMRoomSync.c). Run it again when a proc is added; the order only has to match
between computers running the same version."""
import re, os
root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
protos = open(os.path.join(root, 'src/game/MMGameProtos.h')).read()
move = sorted(set(re.findall(r'^void\s+(\w+MoveProc)\s*\(\s*SpritePtr\s+\w+\s*\)\s*;', protos, re.M)))
frame = sorted(set(re.findall(r'^void\s+(\w+FrameProc)\s*\(\s*SpritePtr[^;]*long\s*\*\s*\w+\s*\)\s*;', protos, re.M)))
with open(os.path.join(root, 'src/game/MMProcTable.h'), 'w') as f:
    f.write('/* MMProcTable.h: made by tools/make_proc_table.py from MMGameProtos.h; do not edit. */\n')
    f.write('#define MM_MOVE_PROCS \\\n' + ''.join('    X(%s) \\\n' % n for n in move) + '\n')
    f.write('#define MM_FRAME_PROCS \\\n' + ''.join('    X(%s) \\\n' % n for n in frame) + '\n')
print(len(move), 'move procs,', len(frame), 'frame procs')
