#!/usr/bin/env python

from os.path import basename, join
from shutil import copy, copytree

from conf import *

# const
PROJ_EXTN = "acroplugin"
PROJ_FULL = "%s.%s" % (PROJ_NAME, PROJ_EXTN)
PROJ_ARCH = raw_input("arch: ") 

# paths
FILE_PINF = join(PATH_INF, "Info.plist")
FILE_SYMS = join(PATH_INF, "Symbols.exp")
FILE_LOCL = join(PATH_INF, "English.lproj")

PATH_EXEC = join(PATH_OBJ, PROJ_NAME)
PATH_PROD = join(PATH_BIN, PROJ_FULL)
PATH_LOAD = "/Users/meng/Library/Application Support/Adobe/Acrobat/DC/Plug-ins"

# lists
LIST_DEFS = COMMON_DEFS
LIST_DEFS.extend([
    "MAC_PLATFORM",
    "DISABLECPLUSPLUSDURING=0",
    "EXCEPTIONS_ALWAYS_CPLUSPLUS=1"
    ])

LIST_INCS = COMMON_INCS

LIST_SRCS = COMMON_SRCS

LIST_PCHS = COMMON_PCHS

# confs
CONF_CC = [
        "clang",
        "-x objective-c++",
        "-arch %s" % PROJ_ARCH,
        "-O0",
        "-isysroot %s" % PATH_OST,
        "-Wno-comment"
        ]

CONF_LD = [
        "clang++",
        "-bundle",
        "-arch %s" % PROJ_ARCH,
        "-dead_strip",
        "-isysroot %s" % PATH_OST,
        "-fobjc-link-runtime"
        ]

# comps
COMP_OBJS = []

# core
def comp_one(src):
    fin = basename(src)
    fout = fin.split(".")[0] + ".o"
    fn = join(PATH_OBJ, fout)

    cmd = \
            " ".join(CONF_CC) + " " + \
            " ".join(["-D%s" % e for e in LIST_DEFS]) + " " + \
            " ".join(["-I%s" % e for e in LIST_INCS]) + " " + \
            " ".join(["-include %s" % e for e in LIST_PCHS]) + " " + \
            "-c %s" % src + " " + \
            "-o %s" % fn

    shell(cmd)
    COMP_OBJS.append(fn)

def comp():
    for e in LIST_SRCS:
        comp_one(e)

def link():
    cmd = \
            " ".join(CONF_LD) + " " + \
            "-exported_symbols_list %s" % FILE_SYMS + " " + \
            " ".join(COMP_OBJS) + " " + \
            "-o %s" % PATH_EXEC

    shell(cmd)

def bdle():
    path_root = join(PATH_BIN, "%s.%s" % (PROJ_NAME, PROJ_EXTN))
    path_base = join(path_root, "Contents")
    path_code = join(path_base, "MacOS")
    path_ress = join(path_base, "Resources")

    create(path_base)
    create(path_code)
    create(path_ress)

    copy(PATH_EXEC, path_code)
    copy(FILE_PINF, path_base)
    copytree(FILE_LOCL, join(path_ress, basename(FILE_LOCL)))

def sign():
    cmd = "%s -kp %s -cf %s %s" % (
            join(PATH_KEY, "sign"),
            join(PATH_KEY, "keypair"), join(PATH_KEY, "cert"),
            PATH_PROD)

    shell(cmd)

