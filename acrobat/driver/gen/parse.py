#!/usr/bin/env python

import re
import os
import sys

OPT_PTN_POKE = re.compile(
        "^POKE\s*\(\s*(\w+?)\s*,\s*\(.+?\),\s*\((.+?)\),\s*\(.+?\)\s*\)$"
        )

OPT_BLACKLIST = [
        "PDDocPermsReady",
        "PDEContainerXAPMetadataDidChange",
        "PDPageGetPrintMatrix",
        "AVDocWillRefreshUI"
        ]

if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit("Usage: %s <path-to PIPokes.h>" % __file__)

    pn = sys.argv[1]

    if not os.path.exists(pn):
        sys.exit("%s does not exist" % pn)

    pokes = []

    with open(pn, "r") as f:
        cur = None 
        for l in f:
            l = l.rstrip()
            if cur is None:
                if not l.startswith("POKE"):
                    continue

                m = OPT_PTN_POKE.match(l)
                if m is not None:
                    pokes.append({
                        "name": m.group(1), 
                        "args": m.group(2),
                        "full": m.group(0)
                        })
                else:
                    if l.endswith("\\"):
                        l = l[:-1]
                
                    cur = l

            else:
                if l.endswith("\\"):
                    l = l[:-1]
                
                cur = cur + l
                m = OPT_PTN_POKE.match(cur)
                if m is not None:
                    pokes.append({
                        "name": m.group(1).strip(),
                        "args": m.group(2).strip(),
                        "full": m.group(0)
                        })

                    cur = None

    for p in pokes:
        if p["name"].startswith("AVApp"):
            continue

        if p["name"] in OPT_BLACKLIST:
            continue

        print "void %s(%s)" % (p["name"].strip(), p["args"].strip())
