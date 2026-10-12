// Applies struct layouts and function prototypes to the analyzed program.
//
// Args: <types.txt> <prototypes.h>...
//
// types.txt format (one directive per line, '#' comments):
//   typedef <NewName> <existing type>        e.g. typedef SpritePtr SpriteRec *
//   struct <Name> <size>                     starts a struct (size in hex 0x.. or decimal)
//   <offset> <type> <fieldName>              field inside the current struct
//   funcdef <prototype>;                     function pointer type, e.g. funcdef void MoveProcPtr(SpriteRec *s);
//
// Structs are created before fields are filled, so fields may point to any struct.
// prototypes.h holds one C prototype per line; functions are matched by name with or
// without the leading underscore Mach-O adds.
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.app.cmd.function.ApplyFunctionSignatureCmd;
import ghidra.app.services.DataTypeManagerService;
import ghidra.app.util.parser.FunctionSignatureParser;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.data.DataTypeParser;
import ghidra.util.data.DataTypeParser.AllowedDataTypes;
import java.io.*;
import java.nio.file.*;
import java.util.*;

public class ApplyTypes extends GhidraScript {
    private DataTypeManager dtm;
    private final CategoryPath CAT = new CategoryPath("/mm");

    private DataType parseType(String s) throws Exception {
        s = s.trim();
        DataTypeParser p = new DataTypeParser(dtm, dtm, null, AllowedDataTypes.ALL);
        return p.parse(s);
    }

    private static int num(String s) {
        s = s.trim().toLowerCase();
        return s.startsWith("0x") ? Integer.parseInt(s.substring(2), 16) : Integer.parseInt(s);
    }

    private DecompInterface decomp;

    /** Parameter count as the decompiler infers it (the database often has none). */
    private int inferredParamCount(Function f) {
        if (f.getParameterCount() > 0 || f.getSignatureSource() != SourceType.DEFAULT) return f.getParameterCount();
        if (decomp == null) {
            decomp = new DecompInterface();
            decomp.openProgram(currentProgram);
        }
        DecompileResults r = decomp.decompileFunction(f, 60, monitor);
        if (r == null || r.getHighFunction() == null) return -1;
        return r.getHighFunction().getFunctionPrototype().getNumParams();
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        dtm = currentProgram.getDataTypeManager();
        List<String> lines = Files.readAllLines(Paths.get(args[0]));

        // Pass 1: declare structs (sized, empty) and typedef placeholders.
        for (String raw : lines) {
            String l = raw.replaceAll("#.*", "").trim();
            if (l.startsWith("struct ")) {
                String[] p = l.split("\\s+");
                StructureDataType st = new StructureDataType(CAT, p[1], num(p[2]), dtm);
                dtm.addDataType(st, DataTypeConflictHandler.REPLACE_HANDLER);
            }
        }
        // Pass 2: function pointer typedefs and plain typedefs, in file order.
        FunctionSignatureParser fsp = new FunctionSignatureParser(dtm, null);
        for (String raw : lines) {
            String l = raw.replaceAll("#.*", "").trim();
            if (l.startsWith("funcdef ")) {
                String proto = l.substring(8).replaceAll(";\\s*$", "");
                FunctionDefinitionDataType fd = fsp.parse(null, proto);
                fd.setCategoryPath(CAT);
                DataType added = dtm.addDataType(fd, DataTypeConflictHandler.REPLACE_HANDLER);
                // Also add a pointer typedef "<Name>" -> pointer to function, named like SW's xxxProcPtr.
                dtm.addDataType(new TypedefDataType(CAT, fd.getName() + "_t", new PointerDataType(added, dtm), dtm),
                        DataTypeConflictHandler.REPLACE_HANDLER);
            } else if (l.startsWith("typedef ")) {
                String[] p = l.split("\\s+", 3);
                DataType base = parseType(p[2]);
                dtm.addDataType(new TypedefDataType(CAT, p[1], base, dtm), DataTypeConflictHandler.REPLACE_HANDLER);
            }
        }
        // Pass 3: fill fields.
        Structure cur = null;
        int nfields = 0;
        for (String raw : lines) {
            String l = raw.replaceAll("#.*", "").trim();
            if (l.isEmpty() || l.startsWith("typedef ") || l.startsWith("funcdef ")) continue;
            if (l.startsWith("struct ")) {
                cur = (Structure) dtm.getDataType(CAT, l.split("\\s+")[1]);
                continue;
            }
            // <offset> <type...> <name>
            String[] p = l.split("\\s+");
            int off = num(p[0]);
            String name = p[p.length - 1];
            String type = l.substring(l.indexOf(p[0]) + p[0].length(), l.lastIndexOf(name)).trim();
            DataType dt;
            try {
                dt = parseType(type);
            } catch (Exception e) {
                printerr("bad type '" + type + "' for " + cur.getName() + "." + name + ": " + e.getMessage());
                continue;
            }
            try {
                cur.replaceAtOffset(off, dt, dt.getLength(), name, null);
                nfields++;
            } catch (Exception e) {
                printerr("cannot place " + cur.getName() + "." + name + " @0x" + Integer.toHexString(off) + ": " + e.getMessage());
            }
        }
        println("fields applied: " + nfields);

        // Prototypes.
        Map<String, Function> byName = new HashMap<>();
        for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
            String n = f.getName();
            byName.putIfAbsent(n, f);
            if (n.startsWith("_")) byName.putIfAbsent(n.substring(1), f);
        }
        int ok = 0, miss = 0, bad = 0, skipped = 0;
        List<String> protoLines = new ArrayList<>();
        for (int i = 1; i < args.length; i++) protoLines.addAll(Files.readAllLines(Paths.get(args[i])));
        for (String raw : protoLines) {
            String l = raw.replaceAll("//.*", "").trim();
            if (l.isEmpty() || l.startsWith("#")) continue;
            // A leading '!' forces the prototype; otherwise it is only applied when its
            // parameter count matches what analysis found (guards against fork changes).
            boolean force = l.startsWith("!");
            if (force) l = l.substring(1).trim();
            l = l.replaceAll(";\\s*$", "");
            FunctionDefinitionDataType sig;
            try {
                sig = fsp.parse(null, l);
            } catch (Exception e) {
                printerr("bad prototype: " + l + " : " + e.getMessage());
                bad++;
                continue;
            }
            Function f = byName.get(sig.getName());
            if (f == null) f = byName.get("_" + sig.getName());
            if (f == null) { miss++; continue; }
            int have = inferredParamCount(f);
            if (!force && have != sig.getArguments().length) {
                println("skip (param count " + have + " vs " + sig.getArguments().length + "): " + l);
                skipped++;
                continue;
            }
            sig.setName(f.getName());
            ApplyFunctionSignatureCmd cmd = new ApplyFunctionSignatureCmd(f.getEntryPoint(), sig, SourceType.USER_DEFINED);
            if (cmd.applyTo(currentProgram, monitor)) ok++; else { printerr("apply failed: " + l + " " + cmd.getStatusMsg()); bad++; }
        }
        println("prototypes applied=" + ok + " missing=" + miss + " skipped=" + skipped + " bad=" + bad);
    }
}
