import sys
import os
from os import listdir
from os.path import isfile, join, splitext
import subprocess

if __name__ == '__main__':
    abc_solver_binary_path = sys.argv[1]
    test_dir = sys.argv[2]
    test_file = {int(splitext(f)[0]) :join(test_dir, f) for f in listdir(test_dir) if isfile(join(test_dir, f))}
    pa = {}
    for k,v in test_file.items():
        pa[k]=k
    for k1, v1 in test_file.items():
        for k2, v2 in test_file.items():
            if k1==k2:
                continue
            output = subprocess.check_output(
                "{} -c 'miter {} {}; strash; dsat'".format(abc_solver_binary_path, v1, v2),
                shell=True)
            rv = True if str(output).find("UNSATISFIABLE")!=-1 else False
            if rv:
                if k1 < k2:
                    pa[k2] = pa[k1]
                else:
                    pa[k1] = pa[k2]
    sets = []
    for k,v in pa.items():
        if k == v:
            temp = set()
            for k2, v2 in pa.items():
                if v2 == v:
                    temp.add(k2)
            sets.append(temp)
    print(sets)
                
                