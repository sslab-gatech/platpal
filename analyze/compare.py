#!/usr/bin/env python

import sys

import plugin

def process(prod, hashes):
    for h in hashes:
        try:
            result = plugin.compare_plats(prod, h)
        except:
            result = None

        if result is not None:
            print "=== %s ===" % h 
            print result

if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit("Usage: %s <list> <product>" % sys.argv[0])

    pn = sys.argv[1]
    prod = sys.argv[2]

    with open(pn, "r") as f:
        hashes = [l.strip() for l in f]

    process(prod, hashes)
