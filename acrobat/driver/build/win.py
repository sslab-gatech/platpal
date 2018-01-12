#!/usr/bin/env python

import os
from os.path import basename, join
from shutil import copy, copytree

from conf import *

# const
PROJ_EXTN = "api"
PROJ_FULL = "%s.%s" % (PROJ_NAME, PROJ_EXTN)
PROJ_ARCH = "x86"

# paths
FILE_SYMS = join(PATH_INF, "Symbols.exp")

PATH_DLIB = join(PATH_OBJ, "%s.lib" % PROJ_NAME)
PATH_EXEC = join(PATH_OBJ, PROJ_NAME)
PATH_PROD = join(PATH_BIN, PROJ_FULL)
PATH_LOAD = "/cygdrive/c/Program Files (x86)/Adobe/Acrobat Reader DC/Reader/plug_ins/"

# lists
LIST_DEFS = COMMON_DEFS
LIST_DEFS.extend([
    "ACRO_SDK_LEVEL=0x00090000",
    "WIN_PLATFORM",
    "WIN_ENV",
    "WIN32",
    "_WINDLL",
    "_WINDOWS"
    ])

LIST_INCS = COMMON_INCS
LIST_INCS.extend([
    join(PATH_OST, "include", "ucrt"),
    join(PATH_OST, "include", "um"),
    join(PATH_OST, "include", "winrt"),
    join(PATH_OST, "include", "shared"),
    join(PATH_CRT, "include")
    ])

LIST_SRCS = COMMON_SRCS

LIST_LIBS = [
        join(PATH_OST, "lib", "ucrt", PROJ_ARCH),
        join(PATH_OST, "lib", "um", PROJ_ARCH),
        join(PATH_CRT, "lib")
        ]

LIST_LSYS = [
        "odbc32.lib",
        "odbccp32.lib",
        "kernel32.lib",
        "user32.lib",
        "gdi32.lib",
        "winspool.lib",
        "comdlg32.lib",
        "advapi32.lib",
        "shell32.lib",
        "ole32.lib",
        "oleaut32.lib",
        "uuid.lib"
        ]

LIST_PCHS = COMMON_PCHS

# confs
CONF_CC = [
        "cl",
        "/c",
        "/Od /Oy-",
        "/Gd",
        "/MTd"
        ]

CONF_LD = [
        "link",
        "/DLL",
        "/MACHINE:X86",
        "/SUBSYSTEM:WINDOWS",
        "/TLBID:1",
        "/NXCOMPAT",
        "/DYNAMICBASE",
        "/MANIFEST",
        "\"/MANIFESTUAC:level='asInvoker' uiAccess='false'\"",
        "/manifest:embed"
        ]

# comps
COMP_OBJS = []

# utils
def winpath(pn):
    return check_output("cygpath -w %s" % pn, shell=True).strip()

# core
def comp_one(src):
    fin = basename(src)
    fout = fin.split(".")[0] + ".o"
    fn = join(PATH_OBJ, fout)

    cmd = \
            " ".join(CONF_CC) + " " + \
            " ".join(["/D %s" % e for e in LIST_DEFS]) + " " + \
            " ".join(["/I\"%s\"" % winpath(e) for e in LIST_INCS]) + " " + \
            "/Fo\"%s\"" % winpath(fn) + " " + \
            "\"%s\"" % winpath(src)

    shell(cmd)
    COMP_OBJS.append(fn)

def comp():
    for e in LIST_SRCS:
        comp_one(e)

def link():
    binpath = abspath(join(PATH_OST, "bin", "x86"))
    os.environ["PATH"] = winpath(binpath) + ":" + os.environ["PATH"]

    with open(FILE_SYMS, "r") as f:
        syms = [e.strip() for e in f.readlines()]

    cmd = \
            " ".join(CONF_LD) + " " + \
            " ".join(["/LIBPATH:\"%s\"" % winpath(e) for e in LIST_LIBS]) + " " + \
            "/INCREMENTAL %s" % " ".join(LIST_LSYS) + " " + \
            " ".join(["/EXPORT:%s" % e for e in syms]) + " " + \
            "/IMPLIB:\"%s\"" % winpath(PATH_DLIB) + " " + \
            "/OUT:\"%s\"" % winpath(PATH_EXEC) + " " + \
            " ".join(["\"%s\"" % winpath(e) for e in COMP_OBJS])

    shell(cmd)

def bdle():
    create(PATH_BIN);
    copy(PATH_EXEC, PATH_PROD)

def sign():
    cmd = "%s -kp \"%s\" -cf \"%s\" \"%s\"" % (
            join(PATH_KEY, "sign"),
            winpath(join(PATH_KEY, "keypair")),
            winpath(join(PATH_KEY, "cert")),
            winpath(PATH_PROD))

    shell(cmd)

