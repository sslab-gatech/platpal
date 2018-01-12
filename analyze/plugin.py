#!/usr/bin/env python

import re
import sys
from os.path import exists, join
from collections import OrderedDict

import conf

# consts
OPT_SUFFIX = "log"

# regex patterns
RE_LINE_PTN         = re.compile("^\[(.*? .*?).*?\] (\d+/\d+) (\w+) (.*)$")

RE_SEC_PTN          = re.compile("^([\w-]+):(\w+)$")
RE_CBK_PTN          = re.compile("^(\w+)$")
RE_AS_PTN           = re.compile("^(\w+)\((.*?)\)$")
RE_COS_DATA_PTN     = re.compile("^(\d+) (\w+) (.*) \((.*)\)$")
RE_COS_META_PTN     = re.compile("^(\w+):(.*)$")
RE_CONS_SCAN_PTN    = re.compile("^\[(\d+)-(\d+)\]\[(\d+)\](.*)$")
RE_PD_DATA_PTN      = re.compile("^\((\w+)\)(.*)$")
RE_PD_META_PTN      = re.compile("^(\w+):(.*)$")
RE_FORM_ELEM_PTN    = re.compile("^\[(.*)\]\"(.*?)\"=\"(.*)\"$")
RE_JS_DUMP_PTN      = re.compile("^DUMP:(\d+):(\d+)$")
RE_JS_STCK_PTN      = re.compile("^STCK:(.*)$")
RE_JS_EXEC_PTN      = re.compile("^EXEC:(\w+)$")

# utils
def path_raw(plat, tag, sample):
    px = conf.prefix(plat, tag, sample)
    return join(conf.OPT_PATH_RAW, "%s.%s" % (px, OPT_SUFFIX))

# structs
class Record:
    def __init__(self, cat, sub, ptid):
        self.cat = cat
        self.sub = sub
        self.ptid = ptid

    def match(self, other):
        return self.cat == other.cat and self.sub == other.sub

class Record_CBK(Record):
    def __init__(self, ptid, name):
        Record.__init__(self, "CBK", "", ptid)
        self.name = name

    def match(self, other):
        return Record.match(self, other) and \
                self.name == other.name

class Record_AS_RW(Record):
    def __init__(self, ptid, path, act, pos, cnt):
        Record.__init__(self, "AS", "RW", ptid)
        self.path = path
        self.act = act 
        self.pos = pos
        self.cnt = cnt

    def match(self, other):
        return Record.match(self, other) and \
                self.path == other.path and self.act == other.act and \
                self.pos == other.pos and self.cnt == other.cnt

class Record_AS_OC(Record):
    def __init__(self, ptid, path, act):
        Record.__init__(self, "AS", "OC", ptid)
        self.path = path
        self.act = act

    def match(self, other):
        return Record.match(self, other) and \
                self.path == other.path and self.act == other.act

class Record_COS_Data(Record):
    def __init__(self, ptid, oid, otype, content, collects):
        Record.__init__(self, "COS", "Data", ptid)
        self.oid = oid
        self.otype = otype
        self.content = content
        self.collects = collects

    def match(self, other):
        return Record.match(self, other) and \
                self.oid == other.oid and self.otype == other.otype and \
                self.content == other.content and \
                self.collects == other.collects

class Record_COS_Meta(Record):
    def __init__(self, ptid, key, val):
        Record.__init__(self, "COS", "Meta", ptid)
        self.key = key
        self.val = val

    def match(self, other):
        return Record.match(self, other) and \
                self.key == other.key and self.val == other.val

class Record_CONS_Scan(Record):
    def __init__(self, ptid, otype, ocat, stack, content):
        Record.__init__(self, "CONS", "Scan", ptid)
        self.otype = otype
        self.ocat = ocat
        self.stack = stack
        self.content = content

    def match(self, other):
        return Record.match(self, other) and \
                self.otype == other.otype and self.ocat == other.ocat and \
                self.stack == other.stack and self.content == other.content

class Record_PD_Data(Record):
    def __init__(self, ptid, otype, content):
        Record.__init__(self, "PD", "Data", ptid)
        self.otype = otype
        self.content = content

    def match(self, other):
        return Record.match(self, other) and \
                self.otype == other.otype and self.content == other.content

class Record_PD_Meta(Record):
    def __init__(self, ptid, key, val):
        Record.__init__(self, "PD", "Meta", ptid)
        self.key = key
        self.val = val

    def match(self, other):
        return Record.match(self, other) and \
                self.key == other.key and self.val == other.val

class Record_Form_Elem(Record):
    def __init__(self, ptid, key, val):
        Record.__init__(self, "Form", "Elem", ptid)
        self.key = key
        self.val = val

    def match(self, other):
        return Record.match(self, other) and \
                self.key == other.key and self.val == other.val

class Record_JS_DUMP(Record):
    def __init__(self, ptid, otype, count):
        Record.__init__(self, "JS", "DUMP", ptid)
        self.otype = otype
        self.count = count
        self.content = None

    def setContent(self, content):
        self.content = content

    def match(self, other):
        return Record.match(self, other) and \
                self.otype == other.otype and self.count == other.count and \
                self.content == other.content

class Record_JS_STCK(Record):
    def __init__(self, ptid, content):
        Record.__init__(self, "JS", "STCK", ptid)
        self.content = content

    def match(self, other):
        return Record.match(self, other) and \
                self.content == other.content

class Record_JS_EXEC(Record):
    def __init__(self, ptid, otype):
        Record.__init__(self, "JS", "EXEC", ptid)
        self.otype = otype
        self.content = None

    def setContent(self, content):
        self.content = content

    def match(self, other):
        return Record.match(self, other) and \
                self.otype == other.otype and \
                self.content == other.content

# transform
def transform(pn):
    content = []
    fixes = set()

    cur = None
    with open(pn, "r") as f:
        for l in f:
            l = l.rstrip()
            if cur is not None:
                if l == ">>>":
                    idx = len(content)
                    fixes.add(idx)
                    fixes.add(idx + 1)
                    content.append("<<<>>>".join(cur))
                    cur = None
                else:
                    cur.append(l)
                continue

            if l == "<<<":
                cur = []
                continue

            content.append(l)

    result = []
    for i, l in enumerate(content):
        if i in fixes:
            result[-1] = result[-1] + l
        else:
            result.append(l)

    return result 

# parser
def parse_asdoc(ptid, act, args):
    if act == "open" or act == "close":
        path = args.split(",")[0].strip().split("/")[-1]
        return Record_AS_OC(ptid, path, act) 

    elif act == "read" or act == "write":
        toks = args.split(",")
        path = toks[0].strip().split("/")[-1]
        pos = int(toks[1].strip())
        cnt = int(toks[3].strip())
        return Record_AS_RW(ptid, path, act, pos, cnt)

    else:
        raise Exception("Unknown action: %s" % act)

def parse_cosdoc(ptid, content):
    m = RE_COS_DATA_PTN.match(content)
    if m is not None:
        return Record_COS_Data(ptid, 
                m.group(1), m.group(2), m.group(3), m.group(4))

    m = RE_COS_META_PTN.match(content)
    if m is not None:
        return Record_COS_Meta(ptid, m.group(1), m.group(2))

    raise Exception("Invalid COS line: %s" % content)

def parse_cons(ptid, content):
    m = RE_CONS_SCAN_PTN.match(content)
    if m is not None:
        return Record_CONS_Scan(ptid, 
                m.group(1), m.group(2), m.group(3), m.group(4))

    raise Exception("Invalid Cons line: %s" % content)

def parse_pddoc(ptid, content):
    m = RE_PD_DATA_PTN.match(content)
    if m is not None:
        return Record_PD_Data(ptid,
                m.group(1), m.group(2))

    m = RE_PD_META_PTN.match(content)
    if m is not None:
        return Record_PD_Meta(ptid, m.group(1), m.group(2))

    raise Exception("Invalid PD line: %s" % content)

def parse_form(ptid, content):
    m = RE_FORM_ELEM_PTN.match(content)
    if m is not None:
        return Record_Form_Elem(ptid,
                m.group(1), m.group(2))

    raise Exception("Invalid Form line: %s" % content)

def parse_file(pn):
    SECDICT = OrderedDict()
    CURRENT = None

    cont = None 
    pending = None

    for l in transform(pn):
        m = RE_LINE_PTN.match(l)
        if m is None:
            raise Exception("Invalid line: %s" % l)

        ts = m.group(1)
        ptid = m.group(2)
        cat = m.group(3)
        content = m.group(4)

        if cat == "ERR":
            raise Exception("Error detected: %s" % l)

        elif cat == "SEC":
            m = RE_SEC_PTN.match(content)
            if m is None:
                raise Exception("Invalid SEC line: %s" % l)

            if m.group(2) == "BEGIN":
                if CURRENT is not None:
                    raise Exception("Nested sections not allowed: %s" % l)

                if m.group(1) in SECDICT:
                    raise Exception("Repeated section not allowed: %s" % l)

                SECDICT[m.group(1)] = []
                CURRENT = [] 
            elif m.group(2) == "END":
                if CURRENT is None:
                    raise Exception("Section ended without begin: %s" % l)

                if m.group(1) not in SECDICT:
                    raise Exception("Section not captured in table: %s" % l)

                SECDICT[m.group(1)].extend(CURRENT)
                CURRENT = None
            else:
                raise Exception("Invalid section mark: %s" % l)
            
        elif cat == "CBK":
            if CURRENT is None:
                continue

            m = RE_CBK_PTN.match(content)
            if m is None:
                raise Exception("Invalid CBK line: %s" % l)

            CURRENT.append(Record_CBK(ptid, m.group(1)))
            continue

        elif cat == "AS":
            if CURRENT is None:
                continue

            m = RE_AS_PTN.match(content)
            if m is None:
                raise Exception("Invalid AS line: %s" % l)
            
            act = m.group(1)
            args = m.group(2) 

            CURRENT.append(parse_asdoc(ptid, act, args))

        elif cat == "COS":
            CURRENT.append(parse_cosdoc(ptid, content))

        elif cat == "CONS":
            CURRENT.append(parse_cons(ptid, content))

        elif cat == "PD":
            CURRENT.append(parse_pddoc(ptid, content))

        elif cat == "FORM":
            CURRENT.append(parse_form(ptid, content))

        elif cat == "JS":
            m = RE_JS_DUMP_PTN.match(content)
            if m is not None:
                pending = Record_JS_DUMP(ptid, m.group(1), m.group(2))
                CURRENT.append(pending)
                continue

            m = RE_JS_EXEC_PTN.match(content)
            if m is not None:
                pending = Record_JS_EXEC(ptid, m.group(1))
                CURRENT.append(pending)
                continue

            m = RE_JS_STCK_PTN.match(content)
            if m is not None:
                pending = Record_JS_STCK(ptid, m.group(1))
                CURRENT.append(pending)
                continue

            raise Exception("Invalid JS line: %s" % l)

        else:
            raise Exception("Unknown category: %s" % l)

    if CURRENT is not None:
        SECDICT["Unclosed"] = []
        SECDICT["Unclosed"].extend(CURRENT)
    
    return SECDICT

# compare
def compare_records(r1, r2):
    for k in r1:
        if k not in r2:
            return "Section mismatch: %s" % k

        if k == "AVLoad":
            continue

        if k == "Drive":
            continue

        s1 = r1[k]
        s2 = r2[k]

        if len(s1) != len(s2):
            return "Entry number mismatch: %s (%d vs %d)" % \
                    (k, len(s1), len(s2))

        for i in range(len(s1)):
            if not s1[i].match(s2[i]):
                return "Entry mismatch: %s (%s vs %s)" % \
                        (k, s1[i], s2[i])

    return "All matched (%d sections)" % len(r1)

def compare_plats(tag, sample):
    p1 = path_raw("mac", tag, sample)
    p2 = path_raw("win", tag, sample)

    if exists(p1) and exists(p2):
        return compare_records(parse_file(p1), parse_file(p2))
    else:
        return None

# main
if __name__ == "__main__":
    act = sys.argv[1]

    if act == "parse":
        result = parse_file(sys.argv[2])
        for k in result:
            print "%s, %d" % (k, len(result[k]))

    elif act == "compare":
        r1 = parse_file(sys.argv[2])
        r2 = parse_file(sys.argv[3])
        result = compare_records(r1, r2)
        if result is not None:
            print result
