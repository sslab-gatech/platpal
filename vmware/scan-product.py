#!/usr/bin/env python

import os
import sys
import json
from os.path import abspath, basename, dirname, exists, join
from subprocess import call
from collections import OrderedDict

# path
OPT_WKS                 = abspath(dirname(__file__))

OPT_DIR_VTDOC           = join(OPT_WKS, "vtdoc", "analyze")

# enums
OPT_ENUM_PLAT           = ["mac", "win"]

# commands
OPT_CMD_BOX             = join(OPT_WKS, "box.py")

# main
if __name__ == "__main__":
    prod = sys.argv[1]

    fn = join(OPT_DIR_VTDOC, "product", "%s.json" % prod)
    with open(fn, "r") as f:
        cves = json.load(f, object_pairs_hook=OrderedDict)

    for c in cves:
        samples = cves[c]
        for s in samples:
            for p in OPT_ENUM_PLAT:
                cmd = ["python", OPT_CMD_BOX, p, prod, s]
                call(cmd)
