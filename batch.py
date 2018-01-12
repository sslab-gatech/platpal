#!/usr/bin/env python

import os
import sys
import json
from os.path import abspath, basename, dirname, exists, join
from subprocess import call
from collections import OrderedDict

# path
OPT_WKS                 = abspath(dirname(__file__))
OPT_CMD                 = join(OPT_WKS, "workflow.py")

# main
if __name__ == "__main__":
    if len(sys.argv) < 3 :
        sys.exit("Usage: %s <list> <product> <opts>" % sys.argv[0])

    pn = sys.argv[1]
    tag = sys.argv[2]

    with open(pn, "r") as f:
        hashes = [l.strip().split(",")[0] for l in f]

    for h in hashes:
        if len(sys.argv) == 4 and sys.argv[3] == "-v":
            cmd = ["python", OPT_CMD, "both", tag, h, "-v"]
        else:
            cmd = ["python", OPT_CMD, "both", tag, h]

        print "=== %s ===" % h
        call(cmd)

        if len(sys.argv) == 4 and sys.argv[3] == "-a":
            continue
        
        raw_input("\nPress enter for next sample\n")
