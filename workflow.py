#!/usr/bin/env python

import sys
from os.path import abspath, exists, join
from subprocess import check_call
from argparse import ArgumentParser

# paths
OPT_PATH_CUR        = abspath(join(__file__, ".."))
OPT_PATH_RAW        = join(OPT_PATH_CUR, "vmware", "trace")

# cmds
OPT_CMD_VMBOX       = join(OPT_PATH_CUR, "vmware", "box.py")
OPT_CMD_STRACE      = join(OPT_PATH_CUR, "analyze", "strace.py")
OPT_CMD_PLUGIN      = join(OPT_PATH_CUR, "analyze", "plugin.py")

# utils
def prefix(plat, tag, sample):
    if plat == "win":
        return "mpcc-x64-win-10-%s-%s" % (tag, sample)
    elif plat == "mac":
        return "mpcc-x64-mac-11-%s-%s" % (tag, sample)
    else:
        raise Exception("Unknown platform %s" % plat)

def path_raw_strace(plat, tag, sample):
    return join(OPT_PATH_RAW, prefix(plat, tag, sample) + ".out")

def path_raw_plugin(plat, tag, sample):
    return join(OPT_PATH_RAW, prefix(plat, tag, sample) + ".log")

# core
def run_single(plat, tag, sample, force = False):
    try:
        if force:
            check_call([OPT_CMD_VMBOX, plat, tag, sample, "-f"])
        else:
            check_call([OPT_CMD_VMBOX, plat, tag, sample])

    except Exception as err:
        raise err

def run_pair(tag, sample, force = False):
    try:
        for plat in ["mac", "win"]:
            run_single(plat, tag, sample, force = force)

        for plat in ["mac", "win"]:
            check_call([OPT_CMD_STRACE, plat, tag, sample])

        check_call([OPT_CMD_PLUGIN, "compare", 
            path_raw_plugin("mac", tag, sample), 
            path_raw_plugin("win", tag, sample)])

    except Exception as err:
        print err.message

def run_view(tag, sample):
    try:
        for plat in ["mac", "win"]:
            check_call([OPT_CMD_STRACE, plat, tag, sample])
            
        check_call([OPT_CMD_PLUGIN, "compare",
            path_raw_plugin("mac", tag, sample),
            path_raw_plugin("win", tag, sample)])

    except Exception as err:
        print err.message

# main 
if __name__ == "__main__":
    # parser
    parser = ArgumentParser()
    parser.add_argument("plat", help="Platform",
            choices=("mac", "win", "both"))
    parser.add_argument("tag", help="Tag",
            choices=(
                "dc.07.20033",
                "dc.08.20082",
                "dc.09.20069",
                "dc.10.20056",
                "dc.10.20060",
                "dc.16.20039",
                "dc.16.20045",
                "dc.17.20050",
                "15.06.30033",
                "11.0.00",
                "11.0.10",
                "10.0.0",
                "10.1.0",
                "10.1.4"
                ))
    parser.add_argument("sample", help="Sample")
    parser.add_argument("-f", "--force", action="store_true")
    parser.add_argument("-v", "--view", action="store_true")

    args = parser.parse_args()

    # run
    if args.view:
        run_view(args.tag, args.sample)

    else:
        if args.plat == "both":
            run_pair(args.tag, args.sample, args.force)
        else:
            run_single(args.plat, args.tag, args.sample, args.force)
