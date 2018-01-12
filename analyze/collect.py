#!/usr/bin/env python

import sys

import strace

def process(plat, prod, hashes, detail = True):
    fops = set()
    nets = set()
    exes = set()

    abnormal = dict()

    # find suitable parser
    if plat == "mac":
        fn = strace.parse_mac
    elif plat == "win":
        fn = strace.parse_win

    # parse and collect
    for h in hashes:
        r = fn(prod, h)
        if r is None:
            continue

        fops.update(r["fops"])
        nets.update(r["nets"])
        exes.update(r["exes"])

        if len(r["fops"]) == 0 and len(r["nets"]) == 0 and len(r["exes"]) == 0:
            continue

        if h not in abnormal:
            abnormal[h] = {"fops": [], "nets": [], "exes": []} 

        for i in r["fops"]:
            abnormal[h]["fops"].append(i)

        for i in r["nets"]:
            abnormal[h]["nets"].append(i)

        for i in r["exes"]:
            abnormal[h]["exes"].append(i)

    if detail:
        for h in abnormal:
            print "=== %s ===" % h

            for i in abnormal[h]["fops"]:
                print i

            for i in abnormal[h]["nets"]:
                print i

            for i in abnormal[h]["exes"]:
                print i

    else:
        for i in sorted(fops):
            print i
            
        for i in sorted(nets):
            print i

        for i in sorted(exes):
            print i

if __name__ == "__main__":
    if len(sys.argv) < 4:
        sys.exit("Usage: %s <list> <platform> <product> <scan>" % sys.argv[0])

    pn = sys.argv[1]
    plat = sys.argv[2]
    prod = sys.argv[3]

    with open(pn, "r") as f:
        hashes = [l.strip() for l in f]

    if len(sys.argv) == 5 and sys.argv[4] == "-s":
        process(plat, prod, hashes, False)
    else:
        process(plat, prod, hashes, True)
