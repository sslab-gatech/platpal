#!/usr/bin/env python

import os
import sys
import json
from os.path import abspath, basename, dirname, exists, join
from subprocess import call
from collections import OrderedDict

# path
OPT_WKS                 = abspath(dirname(__file__))
OPT_CMD_BOX             = join(OPT_WKS, "box.py")

# main
if __name__ == "__main__":
    if len(sys.argv) < 4 :
        sys.exit("Usage: %s <list> <platform> <product> <opts>" % sys.argv[0])

    pn = sys.argv[1]
    plat = sys.argv[2]
    prod = sys.argv[3]

    with open(pn, "r") as f:
        hashes = [l.strip() for l in f]

    for h in hashes:
        cmd = ["python", OPT_CMD_BOX, plat, prod, h]

        print "=== %s ===" % h
        call(cmd)

        if len(sys.argv) == 5 and sys.argv[4] == "-a":
            continue

        raw_input("\nPress enter for next sample\n")
