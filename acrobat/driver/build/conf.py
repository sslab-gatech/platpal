#!/usr/bin/env python

from os import makedirs
from os.path import abspath, exists, join
from platform import system
from shutil import rmtree
from subprocess import check_output, CalledProcessError
from sys import exit

# host
HOST = system()
if HOST == "Darwin":
    HOST = "mac"
    SHARED = "/Users/meng/Shared/build/mac/"
elif HOST == "CYGWIN_NT-10.0":
    HOST = "win"
    SHARED = "/cygdrive/z/shared/build/win/"
else:
    exit("Unknown host: %s" % HOST)

# consts
PROJ_NAME = "Driver"

# paths
PATH_CUR = abspath(join(__file__, ".."))
PATH_OBJ = abspath(join(PATH_CUR, "obj"))
PATH_BIN = abspath(join(PATH_CUR, "bin"))
PATH_INF = abspath(join(PATH_CUR, "inf", HOST))
PATH_SRC = abspath(join(PATH_CUR, ".."))
PATH_DEP = abspath(join(PATH_CUR, "..", "..", HOST))
PATH_OST = abspath(join(PATH_DEP, "ost", "link"))
PATH_CRT = abspath(join(PATH_DEP, "crt", "link"))
PATH_SDK = abspath(join(PATH_DEP, "sdk", "link"))
PATH_KEY = abspath(join(PATH_DEP, "key"))

# lists
COMMON_DEFS = [
        "PDMETADATA_HFT=1",
        "PLUGIN=1",
        "READER_PLUGIN=1",
        "AS_DEBUG=1",
        "DEBUG=1",
        "_DEBUG=1",
        ]

COMMON_INCS = [
        join(PATH_SDK, "SDK"),
        join(PATH_SDK, "API"),
        PATH_SRC
        ]

COMMON_SRCS = [
        join(PATH_SRC, "log.cpp"),
        join(PATH_SRC, "util.cpp"),
        join(PATH_SRC, "fs.cpp"),
        join(PATH_SRC, "cos.cpp"),
        join(PATH_SRC, "cons.cpp"),
        join(PATH_SRC, "pd.cpp"),
        join(PATH_SRC, "form.cpp"),
        join(PATH_SRC, "act.cpp"),
        join(PATH_SRC, "driver.cpp"),
        join(PATH_SDK, "API", "PIMain.c")
        ]

COMMON_PCHS = [
        join(PATH_SDK, "SDK", "PIHeaders++.pch")
        ]

# utils
def create(pn):
    if exists(pn):
        rmtree(pn)

    try:
        makedirs(pn)
    except Exception as err:
        print err
        exit("Script stopped with error")

def shell(cmd):
    print cmd

    try:
        output = check_output(cmd, shell=True)
        result = 0
    except CalledProcessError as err:
        output = err.output
        result = err.returncode

    if isinstance(output, basestring):
        print output
    else:
        for l in output:
            print l

    if result != 0:
        exit("Script stoped with error")
