#!/usr/bin/env python

import os
import re
import sys
from os.path import exists, join, getsize

import conf

# consts
OPT_SUFFIX = "log"

# regex patterns
RE_LINE_PTN         = re.compile("^\[(.*? .*?).*?\] (\d+/\d+) (\w+) (.*)$")
RE_CONS_PTN         = re.compile("^\[(\d+)-(\d+)\]\[(\d+)\](.*)$")

# utils
def path_raw(plat, tag, sample):
    px = conf.prefix(plat, tag, sample)
    return join(conf.OPT_PATH_RAW, "%s.%s" % (px, OPT_SUFFIX))

def path_dump(plat, tag, sample):
    px = conf.prefix(plat, tag, sample)
    return join(conf.OPT_PATH_MID, "%s.%s" % (px, OPT_SUFFIX))

def parse(pn):
    CONTENT = []

    SECTION = False
    CURRENT = None

    with open(pn, "r") as f:
        for l in f:
            l = l.rstrip()

            if CURRENT is not None:
                if l == ">>>":
                    if SECTION:
                        CONTENT.append("\n".join(CURRENT))
                        SECTION = False

                    CURRENT = None
                else:
                    CURRENT.append(l)

                continue

            if l == "<<<":
                CURRENT = []
                continue
        
            m = RE_LINE_PTN.match(l)
            if m is None:
                continue

            cat = m.group(3)
            content = m.group(4)

            if cat == "CONS":
                m = RE_CONS_PTN.match(content)
                if m is None:
                    raise Exception("Invalid CONS line: %s" % l)

                if m.group(1) == "225":
                    SECTION = True

    return CONTENT

def dump(content, pn):
    with open(pn, "w") as f:
        for l in content:
            if "var" not in l and "function" not in l:
                continue

            if l[0] == '"' and l[-1] == '"':
                l = l[1:-1]

            print >> f, "<<<<<\n%s\n>>>>>\n" % l

    if getsize(pn) == 0:
        os.remove(pn)

# main
if __name__ == "__main__":
    if len(sys.argv) < 3:
        sys.exit("Usage: %s <list> <product> <platform>" % sys.argv[0])

    pn = sys.argv[1]
    tag = sys.argv[2]
    plat = sys.argv[3]

    with open(pn, "r") as f:
        hashes = [l.strip().split(",")[0] for l in f]

        for h in hashes:
            praw = path_raw(plat, tag, h)
            if not exists(praw):
                continue

            pdump = path_dump(plat, tag, h)

            result = parse(praw)
            if len(result) == 0:
                continue

            dump(result, pdump)
