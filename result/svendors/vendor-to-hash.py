#!/usr/bin/env python

import re
import sys
from os.path import abspath, dirname, exists, join
from subprocess import call

# path
OPT_DIR = "/Users/meng/workbench/mpcc/vmware/vtdoc/analyze/svendor"

RE_PTN = re.compile("^(\d+),\"(.*)\",\d+$")

# main
if __name__ == "__main__":
    pn = sys.argv[1]

    hashes = set()

    with open(pn, "r") as f:
        for l in f:
            toks = l.strip().split(",")

            vendor = toks[0]
            label = toks[1][1:-1]

            pvd = join(OPT_DIR, vendor)
            if not exists(pvd):
                sys.exit("vendor %s does not exist" % vendor)

            found = None
            with open(join(pvd, "index"), "r") as f:
                for l in f:
                    m = RE_PTN.match(l.strip())
                    if m is None:
                        raise Exception("Invalid line: %s" % l)

                    if m.group(2) == label:
                        found = m.group(1)
                        break

            if found is None:
                sys.exit("label %s does not exist" % label)

            pn = join(pvd, found)
            with open(pn, "r") as f2:
                for l in f2:
                    hashes.add(l.split(",")[0])
        
    for h in hashes:
        print h
