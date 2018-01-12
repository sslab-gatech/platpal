#!/usr/bin/env python

import re
import json
from os.path import exists, join
from argparse import ArgumentParser
from collections import OrderedDict

import conf

# consts
OPT_SUFFIX = "out"

OPT_HOME_MAC = "/Users/bft/"
OPT_HOME_WIN = "C:\\Users\\bft\\"

# utils
def path_raw(plat, tag, sample):
    px = conf.prefix(plat, tag, sample)
    return join(conf.OPT_PATH_RAW, "%s.%s" % (px, OPT_SUFFIX))

def load_filter(pn):
    patterns = []
    with open(pn, "r") as f:
        for l in f:
            l = l.strip()
            if len(l) == 0 or l[0] == '#':
                continue

            patterns.append(re.compile("^" + l + "$"))

            if l.endswith("/.*"):
                patterns.append(re.compile("^" + l[:-3] + "$"))

            if l.endswith("\\\\.*"):
                patterns.append(re.compile("^" + l[:-4] + "$"))

    return patterns

def load_filter_chain(cat, plat, prod):
    patterns = []
    tag = prod[0:2]

    pn = join(conf.OPT_DIR_FILTER, "%s-%s.txt" % (cat, plat))
    if exists(pn):
        patterns.extend(load_filter(pn))

    pn = join(conf.OPT_DIR_FILTER, "%s-%s-%s.txt" % (cat, plat, tag))
    if exists(pn):
        patterns.extend(load_filter(pn))

    pn = join(conf.OPT_DIR_FILTER, "%s-%s-%s.txt" % (cat, plat, prod))
    if exists(pn):
        patterns.extend(load_filter(pn))

    return patterns

# parse
FILTER_FOP_MAC = [] 
FILTER_NET_MAC = []
FILTER_EXE_MAC = []

def decode_mac_fop(pnstr):
    if pnstr[0] != '/':
        pnstr = OPT_HOME_MAC + pnstr

    skip = 0
    toks = []
    
    for t in reversed(pnstr.split("/")):
        if len(t) == 0 or t == ".":
            continue

        if t == "..":
            skip += 1
            continue

        if skip > 0:
            skip -= 1 
            continue

        toks.append(t)

    return "/" + "/".join(reversed(toks))

def filter_mac_fop(pnstr):
    parsed = decode_mac_fop(pnstr)

    for f in FILTER_FOP_MAC:
        if f.match(parsed) is not None:
            return None 
    
    return parsed 

def filter_mac_net(ustr):
    for f in FILTER_NET_MAC:
        if f.match(ustr) is not None:
            return None

    return ustr

def filter_mac_exe(pnstr):
    base = pnstr
    for f in FILTER_EXE_MAC:
        if f.match(base) is not None:
            return None

    return base

def parse_mac_pin(act, args, ret, err):
    return (act == "open") and (conf.OPT_FN_SAMPLE in args)

def parse_mac_exe(act, args, ret, err):
    if act == "exeve" or act == "posix_spawn":
        exe = filter_mac_exe(args[1:-1])
        if exe is not None:
            return {"arg": exe, "ret": ret, "err": err}

    return None

def parse_mac_fop(act, args, ret, err):
    if act == "open" or act == "open_extended" or act == "open_nocancel":
        parg = args[1:-1]
        pn = filter_mac_fop(parg)
        if pn is not None:
            return {"arg": pn, "ret": ret, "err": err}

    elif act == "openat":
        parg = args.split(",")[1].strip()[1:-1]
        pn = filter_mac_fop(parg)
        if pn is not None:
            return {"arg": pn, "ret": ret, "err": err}

    return None

def parse_mac_net(act, args, ret, err):
    if act == "connect" or act == "connect_nocancel":
        net = filter_mac_net(args)
        if net is not None:
            return {"arg": net, "ret": ret, "err": err}

    return None

def parse_mac(tag, sample):
    global FILTER_FOP_MAC
    FILTER_FOP_MAC = load_filter_chain("fop", "mac", tag)
    
    global FILTER_NET_MAC
    FILTER_NET_MAC = load_filter_chain("net", "mac", tag)

    global FILTER_EXE_MAC
    FILTER_EXE_MAC = load_filter_chain("exe", "mac", tag)

    RE_LINE = re.compile("^(\w+)/(\w+):\s+(\w+)\((.*)\)\s=\s(-?\w+)\s(\w+)$")

    pinned = False

    exes = []
    fops = []
    nets = []

    pn = path_raw("mac", tag, sample)
    if not exists(pn):
        return None

    with open(pn, "r") as f:
        content = f.readlines()

    for l in content:
        l = l.strip()
        
        if len(l) == 0:
            continue

        m = RE_LINE.match(l.strip())
        if m is None:
            raise Exception("Unmatched line: %s" % l)

        act = m.group(3)
        args = m.group(4)
        ret = m.group(5)
        err = m.group(6)

        if not pinned:
            pinned = parse_mac_pin(act, args, ret, err)

        if not pinned:
            continue

        op_exe = parse_mac_exe(act, args, ret, err)
        if op_exe is not None:
            exes.append(op_exe)
            continue

        op_fop = parse_mac_fop(act, args, ret, err)
        if op_fop is not None:
            fops.append(op_fop)
            continue

        op_net = parse_mac_net(act, args, ret, err)
        if op_net is not None:
            nets.append(op_net)
            continue

    data = OrderedDict()
    data["plat"] = "mac"
    data["prod"] = tag
    data["exes"] = sorted(set([e["arg"] for e in exes]))
    data["nets"] = sorted(set([e["arg"] for e in nets]))
    data["fops"] = sorted(set([e["arg"] for e in fops]))

    return data


FILTER_FOP_WIN = []
FILTER_NET_WIN = []
FILTER_EXE_WIN = []

def decode_win_fop(pnstr):
    pnstr = pnstr.lower()

    skip = 0
    toks = []
    
    for t in reversed(pnstr.split("\\")):
        if len(t) == 0 or t == ".":
            continue

        if t == "..":
            skip += 1
            continue

        if skip > 0:
            skip -= 1 
            continue

        toks.append(t)

    return "\\".join(reversed(toks))

def filter_win_fop(pnstr):
    parsed = decode_win_fop(pnstr)

    for f in FILTER_FOP_WIN:
        if f.match(parsed) is not None:
            return None 
    
    return parsed 

def filter_win_net(ustr):
    for f in FILTER_NET_WIN:
        if f.match(ustr) is not None:
            return None

    return ustr

def filter_win_exe(pnstr):
    base = pnstr.split("\\")[-1]
    for f in FILTER_EXE_WIN:
        if f.match(base) is not None:
            return None

    return base

def parse_win_pin(act, args, ret, err):
    return (act == "CreateFile") and (conf.OPT_FN_SAMPLE in args)

def parse_win_exe(act, args, ret, err):
    if act == "Process Create":
        if ret == "SUCCESS":
            err = None
        else:
            err = ret
            ret = "FAILURE"

        exe = filter_win_exe(args)
        if exe is not None:
            return {"arg": exe, "ret": ret, "err": err}

    return None

def parse_win_fop(act, args, ret, err):
    if act == "CreateFile" or act == "CreateFileMapping" or act == "Load Image":
        if ret == "SUCCESS":
            err = None
        else:
            err = ret
            ret = "FAILURE"
        
        pn = filter_win_fop(args)
        if pn is not None:
            return {"arg": pn, "ret": ret, "err": err}

    return None

def parse_win_net(act, args, ret, err):
    if act == "TCP Connect":
        args = args.split("->")[1].strip()
        if ret == "SUCCESS":
            err = None
        else:
            err = ret
            ret = "FAILURE"

        url = filter_win_net(args)
        if url is not None:
            return {"arg": url, "ret": ret, "err": err}

    return None

def parse_win(tag, sample):
    global FILTER_FOP_WIN
    FILTER_FOP_WIN = load_filter_chain("fop", "win", tag)
    
    global FILTER_NET_WIN
    FILTER_NET_WIN = load_filter_chain("net", "win", tag)

    global FILTER_EXE_WIN
    FILTER_EXE_WIN = load_filter_chain("exe", "win", tag)

    RE_LINE = re.compile('"(.*?)","(.*?)","(.*?)","(.*?)","(.*?)","(.*?)","(.*?)"')

    pinned = False

    exes = []
    fops = []
    nets = []
 
    pn = path_raw("win", tag, sample)
    if not exists(pn):
        return None

    with open(pn, "r") as f:
        f.readline()
        f.readline()
        content = f.readlines()

    for l in content:
        l = l.strip()

        if len(l) == 0:
            continue

        m  = RE_LINE.search(l)
        if m is None:
            raise Exception("Unmatched line: %s" % l)

        act = m.group(4)
        args = m.group(5)
        ret = m.group(6)
        err = m.group(7)

        if not pinned:
            pinned = parse_win_pin(act, args, ret, err)

        if not pinned:
            continue

        op_exe = parse_win_exe(act, args, ret, err)
        if op_exe is not None:
            exes.append(op_exe)
            continue

        op_fop = parse_win_fop(act, args, ret, err)
        if op_fop is not None:
            fops.append(op_fop)
            continue

        op_net = parse_win_net(act, args, ret, err)
        if op_net is not None:
            nets.append(op_net)
            continue

    data = OrderedDict()
    data["plat"] = "win"
    data["prod"] = tag
    data["exes"] = sorted(set([e["arg"] for e in exes]))
    data["nets"] = sorted(set([e["arg"] for e in nets]))
    data["fops"] = sorted(set([e["arg"] for e in fops]))

    return data

# main
if __name__ == "__main__":
    # parser
    parser = ArgumentParser()
    parser.add_argument("plat", help="Platform",
            choices=("mac", "win"))
    parser.add_argument("tag", help="Tag",
            choices=(
                "dc.07.20033",
                "dc.08.20082",
                "dc.09.20069",
                "dc.10.20056",
                "dc.16.20039",
                "dc.16.20045",
                "dc.17.20050",
                "15.06.30033",
                "11.0.00",
                "11.0.10",
                "10.0.0",
                "10.1.0"
                ))
    parser.add_argument("sample", help="Sample")

    args = parser.parse_args()

    # run
    if args.plat == "mac":
        data = parse_mac(args.tag, args.sample)
    elif args.plat == "win":
        data = parse_win(args.tag, args.sample)
    else:
        raise Exception("Unknown platform %s" % args.plat)

    print json.dumps(data, indent = 2)
