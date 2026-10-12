import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;
import java.util.*;

public class ExportByFile extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        File map = new File(args[0]);
        File outDir = new File(args[1]);
        outDir.mkdirs();
        LinkedHashMap<String, List<String[]>> files = new LinkedHashMap<>();
        Set<Long> mapped = new HashSet<>();
        try (BufferedReader r = new BufferedReader(new FileReader(map))) {
            String line;
            while ((line = r.readLine()) != null) {
                String[] p = line.split("\t");
                files.computeIfAbsent(p[0], k -> new ArrayList<>()).add(p);
                mapped.add(Long.parseLong(p[2], 16));
            }
        }
        DecompInterface di = new DecompInterface();
        DecompileOptions opts = new DecompileOptions();
        di.setOptions(opts);
        di.toggleCCode(true);
        di.openProgram(currentProgram);
        AddressSpace sp = currentProgram.getAddressFactory().getDefaultAddressSpace();
        FunctionManager fm = currentProgram.getFunctionManager();
        int fail = 0, ok = 0;
        for (Map.Entry<String, List<String[]>> e : files.entrySet()) {
            String rel = e.getKey().replace("../", "").replaceAll("^/Users/vernjensen/", "ext/").replaceAll("^/var/tmp/.*/", "ext/");
            File out = new File(outDir, rel);
            out.getParentFile().mkdirs();
            try (PrintWriter w = new PrintWriter(new FileWriter(out))) {
                w.println("// Ghidra decompilation of functions from original source file:");
                w.println("//   " + e.getKey());
                w.println();
                for (String[] p : e.getValue()) {
                    Address a = sp.getAddress(Long.parseLong(p[2], 16));
                    Function f = fm.getFunctionAt(a);
                    if (f == null) {
                        try { f = createFunction(a, p[1].startsWith("_") ? p[1].substring(1) : p[1]); } catch (Exception ex) {}
                    }
                    w.println("// ---- " + p[1] + " @ 0x" + p[2] + " (" + p[3] + " bytes)");
                    if (f == null) { w.println("// <no function>\n"); fail++; continue; }
                    DecompileResults res = di.decompileFunction(f, 120, monitor);
                    if (res != null && res.decompileCompleted()) {
                        w.println(res.getDecompiledFunction().getC());
                        ok++;
                    } else {
                        w.println("// <decompile failed: " + (res == null ? "null" : res.getErrorMessage()) + ">\n");
                        fail++;
                    }
                }
            }
        }
        // Functions not covered by the debug map
        try (PrintWriter w = new PrintWriter(new FileWriter(new File(outDir, "unmapped_functions.txt")))) {
            for (Function f : fm.getFunctions(true)) {
                if (f.isExternal() || f.isThunk()) continue;
                long off = f.getEntryPoint().getOffset();
                if (!mapped.contains(off))
                    w.println(String.format("%08x %6d %s", off, f.getBody().getNumAddresses(), f.getName()));
            }
        }
        println("decompiled ok=" + ok + " fail=" + fail);
    }
}
