#!/usr/bin/env python

from os.path import abspath, exists, join

# paths
OPT_PATH_CUR        = abspath(join(__file__, ".."))
OPT_PATH_RAW        = abspath(join(OPT_PATH_CUR, "..", "vmware", "trace"))
OPT_PATH_MID        = abspath(join(OPT_PATH_CUR, "data", "mid"))
OPT_PATH_END        = abspath(join(OPT_PATH_CUR, "data", "end"))

OPT_DIR_FILTER      = abspath(join(OPT_PATH_CUR, "filter"))

# consts
OPT_FN_SAMPLE       = "Maldoc.pdf"
OPT_FN_TRACE        = "Maldoc.out"
OPT_FN_LOG          = "Maldoc.log"

# utils
def prefix(plat, tag, sample):
    if plat == "win":
        return "mpcc-x64-win-7-%s-%s" % (tag, sample)
    elif plat == "mac":
        return "mpcc-x64-mac-11-%s-%s" % (tag, sample)
    else:
        raise Exception("Unknown platform %s" % plat)
