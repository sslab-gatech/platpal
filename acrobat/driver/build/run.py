#!/usr/bin/env python

import sys
from os import remove
from os.path import exists, isfile, join
from shutil import copy, copytree, rmtree

from conf import *

# set platform
if HOST == "mac":
    from mac import *
elif HOST == "win":
    from win import *
else:
    exit("Unknown host: %s" % HOST)

# paths
FROM = PATH_PROD
DEST = join(PATH_LOAD, PROJ_FULL)

# func
def share():
    DEST = SHARED + "/" + PROJ_ARCH + "/" + PROJ_FULL
    if exists(DEST):
        if isfile(DEST):
            remove(DEST)
        else:
            rmtree(DEST)

    if HOST == "mac":
        copytree(FROM, DEST)
    elif HOST == "win":
        copy(FROM, DEST)
    else:
        exit("Unknown host: %s" % HOST)

def build():
    create(join(PATH_OBJ))

    comp()
    link()
    bdle()
    sign()

    share()

    print "Build finished"

def uninstall():
    if exists(DEST):
        if isfile(DEST):
            remove(DEST)
        else:
            rmtree(DEST)

def install():
    uninstall()

    if isfile(FROM):
        copy(FROM, DEST)
    else:
        copytree(FROM, DEST)

# main
if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit("Usage: %s <build|install|uninstall|share> <opt>" % \
                sys.argv[0])

    cmd = sys.argv[1]

    try:

        if cmd == "build":
            if len(sys.argv) == 3:
                opt = sys.argv[2]
                if opt == "-v":
                    LIST_DEFS.append("OPT_UI_CHECK")
                elif opt == "-f":
                    LIST_DEFS.append("OPT_UI_CHECK")
                    LIST_DEFS.append("OPT_FULL_CHECK")
                else:
                    raise Exception("Invalid option: %s" % opt)

            build()
        elif cmd == "install":
            install()
        elif cmd == "uninstall":
            uninstall()
        elif cmd == "share":
            share()
        else:
            sys.exit("Unknown command: %s" % sys.argv[1])

    except OSError as err:
        sys.exit(err.strerror)
    except IOError as err:
        sys.exit(err.strerror)
