#!/usr/bin/env python

import re
import sys
from os.path import abspath, dirname, exists, join
from subprocess import call

# path
OPT_WKS                 = abspath(dirname(__file__))
OPT_CMD                 = join(OPT_WKS, "batch.py")
OPT_DIR                 = join(OPT_WKS, "vmware", "vtdoc", "analyze", "svendor")

RE_PTN = re.compile("^(\d+),\"(.*)\",\d+$")

# main
if __name__ == "__main__":
    if len(sys.argv) < 4 :
        sys.exit("Usage: %s <vendor> <label> <prod> <opts>" % sys.argv[0])

    vendor = sys.argv[1]
    label = sys.argv[2]
    prod = sys.argv[3]

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
    cmd = ["python", OPT_CMD, pn, prod]

    print "%s - %s - %s" % (vendor, label, prod)
    call(cmd)
