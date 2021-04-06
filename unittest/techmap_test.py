#!/usr/bin/env python3

import sys
import os
import subprocess

tmp_dir = '/tmp'

if __name__ == '__main__':
    abc_solver_binary_path = sys.argv[1]
    techmap_binary_path = sys.argv[2]
    test_file = sys.argv[3]
    temp_file = os.path.join(tmp_dir, "gen_" + os.path.basename(test_file))
    with open(temp_file, "w") as outfile:
        subprocess.run("{} {}".format(techmap_binary_path, test_file), stdout=outfile, shell=True)
    output = subprocess.check_output(
        "{} -c 'miter {} {}; strash; dsat'".format(abc_solver_binary_path, test_file,
                                                   temp_file),
        shell=True)
    assert str(output).find("UNSATISFIABLE") != -1
