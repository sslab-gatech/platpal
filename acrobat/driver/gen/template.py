#!/usr/bin/env python

import re
from os.path import abspath, dirname, exists, join
from enum import Enum
from collections import OrderedDict

# paths 
OPT_DIR_ROOT            = abspath(dirname(dirname(__file__)))
OPT_DIR_HOOK            = join(OPT_DIR_ROOT, "gen", "hook")

OPT_FILE_LOGIC          = join(OPT_DIR_ROOT, "callback-logic.cpp")
OPT_FILE_REGISTER       = join(OPT_DIR_ROOT, "callback-register.cpp")

OPT_LIST_HOOK           = join(OPT_DIR_ROOT, "gen", "hooks.list")

# code gen
OPT_PREFIX_HOOK         = "on"
OPT_SUFFIX_NSEL         = "NSEL"

# misc
OPT_TAB_SIZE            = 2

# utils
def fcode(code, prefix):
    return (prefix + code.replace("\n", "\n" + prefix)).expandtabs(OPT_TAB_SIZE)

def memberize(members, name, sep):
    stmts = []
    for member in members:
        generated = getattr(member, "gen_%s" % name)()
        if generated is not None and len(generated) != 0:
            stmts.append(generated)

    return sep.join(stmts)

def templatize(obj, path):
    code = []

    with open(path, "r") as f:
        contents = f.readlines()
        for line in contents:
            line = line.strip("\n")
            matches = OPT_PTN_ELEM.findall(line)
            for element in matches:
                target = "{{%s}}" % element
                prefix = line[0:len(line) - len(line.lstrip())]

                generated = getattr(obj, "gen_%s" % element)()
                formatted = fcode(generated, prefix)

                line = line.replace(target, formatted)

            code.append(line)

    return "\n".join(code)

def commentize(objs, func, path):
    stmts = []
    for obj in objs:
        generated = getattr(obj, "gen_%s" % func)()
        formatted = fcode(generated, "")
        stmts.append(formatted)

    with open(path, "w") as f:
        f.writelines("\n".join(stmts))

# core
class Param:
    def __init__(self, defs, name):
        self.defs = defs
        self.name = name

    def gen_def(self):
        return "%s %s" % (self.defs, self.name)

    def gen_use(self):
        return self.name

class Hook:
    def __init__(self, name, *args):
        self.name = name
        self.args = list(args)

        self.args.append(Param("void *", "aux"))

    def gen_name_hook(self):
        return "%s%s" % (OPT_PREFIX_HOOK, self.name)

    def gen_name_nsel(self):
        return "%s%s" % (self.name, OPT_SUFFIX_NSEL)

    def gen_args_def(self):
        return memberize(self.args, "def", ",\n\t\t")

    def gen_args_use(self):
        return memberize(self.args, "use", ", ")

    def gen_proto(self):
        return \
'''ACCB1 void %s(
\t\t%s)''' % (self.gen_name_hook(), self.gen_args_def())

    def gen_logic(self):
        pn = join(OPT_DIR_HOOK, "%s.cpp" % self.name)

        if exists(pn):
            ctnt = templatize(self, pn)
        else:
            ctnt = 'LOG_CBK("%s");' % self.name 

        return \
'''%s
{
\t%s
}''' % (self.gen_proto(), ctnt)

    def gen_register(self):
        return \
'''AVAppRegisterNotification(%s,
\t\tgExtensionID,
\t\t(void *)%s,
\t\tNULL);''' % (self.gen_name_nsel(), self.gen_name_hook())

    def gen_unregister(self):
        return \
'''AVAppUnregisterNotification(%s,
\t\tgExtensionID,
\t\t(void *)%s,
\t\tNULL);''' % (self.gen_name_nsel(), self.gen_name_hook())

# factory
HOOK_PROTO_PATTERN = re.compile(
        "(.+) (\w+)\((.*), void(?:\* | \*)clientData\)"
        )

def parse_hook(proto):
    m = HOOK_PROTO_PATTERN.match(proto)
    if m is None:
        raise Exception("Input does not match: %s" % proto)

    retn = m.group(1)
    name = m.group(2)
    args = m.group(3)

    params = []
    for a in args.split(", "):
        a = a.split(" ")
        params.append(Param(a[0], a[1]))

    return Hook(name, *params)
