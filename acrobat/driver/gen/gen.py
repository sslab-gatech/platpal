#!/usr/bin/env python

from template import *

if __name__ == "__main__":
    group = []

    with open(OPT_LIST_HOOK, "r") as f:
        for line in f:
            line = line.strip()
            hook = parse_hook(line)
            group.append(hook)

    commentize(group, "logic", OPT_FILE_LOGIC)
    commentize(group, "register", OPT_FILE_REGISTER)
