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
OPT_ENUN_TAGS           = ["10", "11", "15", "dc"]

# commands
OPT_CMD_BOX             = join(OPT_WKS, "box.py")

# consts
OPT_REC_LIMIT           = 35

# main
if __name__ == "__main__":
    samples = []

    fn = join(OPT_DIR_VTDOC, "sort-srecents.csv")
    with open(fn, "r") as f:
        for l in f:
            toks = l.strip().split(",")
            if int(toks[1]) > OPT_REC_LIMIT:
                samples.append(toks[0])

    for s in samples:
        for t in OPT_ENUN_TAGS:
            for p in OPT_ENUM_PLAT:
                cmd = ["python", OPT_CMD_BOX, p, t, s]
                call(cmd)
