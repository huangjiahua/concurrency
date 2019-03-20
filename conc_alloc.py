import platform
import os
import multiprocessing

cpu = multiprocessing.cpu_count()
sys = platform.system()
print("OS: " + sys)
print("Hardware concurrency: " + str(cpu))
delim = ""
if sys == "Windows":
    delim = "\\"
else:
    delim = "/"

for j in {8, 16, 32}:
    for i in range(1, cpu + 1):
        os.system("cmake-build-release" + delim + "conc_mem_alloc.exe " + str(i) + " " + str(j))



