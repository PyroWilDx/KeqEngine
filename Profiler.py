#!/usr/bin/env python3

import os

profiler = "gperftools"
exe = "./bin/main"
file = "Profiler"

if profiler == "gprof":
    os.system(f"gprof --exclude=libboost,libSDL2 {exe} gmon.out > {file}.txt")
    os.system(f"gprof2dot {file}.txt -o {file}.dot")
    os.system(f"dot -Tpng {file}.dot -o {file}.png")
    os.system(f"rm {file}.txt")
    os.system(f"rm {file}.dot")
    os.system(f"eog {file}.png")

if profiler == "gperftools":
    os.system(f"google-pprof --pdf {exe} ./bin/{file}.prof > {file}.pdf")
    os.system(f"evince {file}.pdf")
