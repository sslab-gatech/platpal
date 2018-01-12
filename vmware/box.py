#!/usr/bin/env python

import os
import sys
import json
import signal
import requests
from os.path import abspath, basename, dirname, exists, expanduser, getsize, join
from shutil import copy
from time import sleep
from enum import Enum
from hashlib import sha256
from subprocess import call, check_output, Popen, STDOUT
from threading import Thread
from argparse import ArgumentParser

# paths
OPT_WKS                 = abspath(dirname(__file__))

OPT_DIR_VTDOC           = join(OPT_WKS, "vtdoc", "sample")
OPT_DIR_UTILS           = join(OPT_WKS, "utils")
OPT_DIR_TRACE           = join(OPT_WKS, "trace")
OPT_DIR_VIDEO           = join(OPT_WKS, "video")

OPT_DIR_SHARE           = join(expanduser("~"), "Shared", "build")
OPT_DIR_VMCMD           = join("/", "Applications",
                                "VMware Fusion.app", "Contents", "Library")
OPT_DIR_VMIMG           = join(expanduser("~"), "Documents",
                                "Virtual Machines.localized")
OPT_URL_VTDOC           = "http://meng.gtisc.gatech.edu:9000/sample/"

# commands
OPT_CMD_VMRUN           = join(OPT_DIR_VMCMD, "vmrun")

# consts
OPT_VM_USER             = "bft"
OPT_VM_PASS             = "bft"
OPT_VM_LIFE             = 30
OPT_VM_REST             = 5
OPT_VM_SIZE             = 1024 * 1024 * 4 # 4MB
OPT_VM_TASK             = "work"
OPT_VM_PROG             = {
        "mac": {
            "10": "/Applications/Adobe\\ Reader.app/Contents/MacOS/AdobeReader",
            "11": "/Applications/Adobe\\ Reader.app/Contents/MacOS/AdobeReader",
            "15": "/Applications/Adobe\\ Acrobat\\ Reader\\ 2015.app/Contents/MacOS/AdobeReader",
            "dc": "/Applications/Adobe\\ Acrobat\\ Reader\\ DC.app/Contents/MacOS/AdobeReader"
            },
        "win": {
            "10": "C:\\Program Files (x86)\\Adobe\Reader 10.0\\Reader\\AcroRd32.exe",
            "11": "C:\\Program Files (x86)\\Adobe\Reader 11.0\\Reader\\AcroRd32.exe",
            "15": "C:\\Program Files (x86)\\Adobe\Acrobat Reader 2015\\Reader\\AcroRd32.exe",
            "dc": "C:\\Program Files (x86)\\Adobe\Acrobat Reader DC\\Reader\\AcroRd32.exe"
            }
        }
OPT_VM_PLUG             = {
        "mac": {
            "10": "/Users/bft/Library/Application Support/Adobe/Acrobat/10.0/Plug-ins",
            "11": "/Users/bft/Library/Application Support/Adobe/Acrobat/11.0/Plug-ins",
            "15": "/Users/bft/Library/Application Support/Adobe/Acrobat/2015/Plug-ins",
            "dc": "/Users/bft/Library/Application Support/Adobe/Acrobat/DC/Plug-ins"
            },
        "win": {
            "10": "C:\\Program Files (x86)\\Adobe\\Reader 10.0\\Reader\\plug_ins",
            "11": "C:\\Program Files (x86)\\Adobe\\Reader 11.0\\Reader\\plug_ins",
            "15": "C:\\Program Files (x86)\\Adobe\\Acrobat Reader 2015\\Reader\\plug_ins",
            "dc": "C:\\Program Files (x86)\\Adobe\\Acrobat Reader DC\\Reader\\plug_ins"
            }
        }
OPT_VM_ARCH             = {
        "mac": {
            "10": "i386",
            "11": "i386",
            "15": "x86_64",
            "dc": "x86_64"
            },
        "win": {
            "10": "x86",
            "11": "x86",
            "15": "x86",
            "dc": "x86"
            }
        }

# names
OPT_FN_SAMPLE           = "Maldoc.pdf"
OPT_FN_TRACE            = "Maldoc.out"
OPT_FN_LOG              = "Maldoc.log"

# utils
class Command(object):
    def __init__(self, cmd):
        self.cmd = cmd
        self.process = None
        
    def run(self, timeout):
        def target():
            self.process = Popen(self.cmd)
            self.process.communicate()
            
        thread = Thread(target=target)
        thread.start()
        
        thread.join(timeout)
        if thread.is_alive():
            self.process.terminate()
            thread.join()

        return self.process.returncode

def exec_and_wait(cmd):
    return call(cmd) == 0

def exec_and_check(cmd):
    try:
        return check_output(cmd)
    except:
        return None

def exec_and_interact(cmd):
    fnull = open(os.devnull, "w")
    return Popen(cmd, stdout=fnull, stderr=STDOUT, env=os.environ)

def exec_and_timeout(cmd, timeout):
    return Command(cmd).run(timeout) == 0

# defs
class Machine:
    def __init__(self, arch, plat, vern, home, bash):
        self.arch = arch
        self.plat = plat
        self.vern = vern
        self.home = home
        self.bash = bash

        self.name = "mpcc-%s-%s-%s" % (arch, plat, vern)
        self.path = join(OPT_DIR_VMIMG,
                "%s.vmwarevm" % self.name, "%s.vmx" % self.name)

    def __prepare_sample(self, sample):
        pn = join(OPT_DIR_VTDOC, sample)

        if not exists(pn):
            r = requests.get(OPT_URL_VTDOC + sample)
            if r.status_code != 200:
                raise Exception("Error downloading sample")

            with open(pn, "w") as f:
                f.write(r.content)

            with open(pn, "r") as f:
                dg = sha256(f.read()).hexdigest()
                if dg != sample:
                    raise Exception("Error matching sample")

        if getsize(pn) > OPT_VM_SIZE:
            raise Exception("File too big")

    def __launch_vm(self, tag):
        # restore snapshot
        cmd = [OPT_CMD_VMRUN, "revertToSnapshot", self.path,
                "%s-%s" % (OPT_VM_TASK, tag)]

        if not exec_and_wait(cmd):
            raise Exception("Error reverting VM")

        # start the vm
        cmd = [OPT_CMD_VMRUN, "start", self.path, "gui"]
        if not exec_and_wait(cmd):
            raise Exception("Error starting VM")

    def __shutdown_vm(self):
        cmd = [OPT_CMD_VMRUN, "stop", self.path, "hard"]
        if not exec_and_wait(cmd):
            raise Exception("Error stopping VM")

    def __mkdir(self, pn):
        cmd = [OPT_CMD_VMRUN,
                "-gu", OPT_VM_USER, "-gp", OPT_VM_PASS,
                "createDirectoryInGuest", self.path,
                pn]

        if not exec_and_wait(cmd):
            raise Exception("Error creating %s in VM" % pn)

    def __transfer(self, pl, pr):
        cmd = [OPT_CMD_VMRUN,
                "-gu", OPT_VM_USER, "-gp", OPT_VM_PASS,
                "copyFileFromHostToGuest", self.path,
                pl, pr]

        if not exec_and_wait(cmd):
            raise Exception("Error creating %s in VM" % pr)

    def __upload(self, fn, pn):
        pvm = self.home % fn

        cmd = [OPT_CMD_VMRUN,
                "-gu", OPT_VM_USER, "-gp", OPT_VM_PASS,
                "copyFileFromHostToGuest", self.path,
                pn, pvm]

        if not exec_and_wait(cmd):
            raise Exception("Error uploading %s to VM" % pn)

        return pvm

    def __download(self, fn, pn):
        pvm = self.home % fn

        cmd = [OPT_CMD_VMRUN,
                "-gu", OPT_VM_USER, "-gp", OPT_VM_PASS,
                "copyFileFromGuestToHost", self.path,
                pvm, pn]

        if not exec_and_wait(cmd):
            raise Exception("Error downloading %s to host" % pn)

        return pvm

    def __prepare_context(self, tag, sample):
        # upload sample
        pvm = self.__upload(OPT_FN_SAMPLE, join(OPT_DIR_VTDOC, sample))

        # upload tracer
        if self.plat == "mac":
            ptracer = self.__upload("DTrace.sh",
                    join(OPT_DIR_UTILS, "DTrace.sh"))

        elif self.plat == "win":
            ptracer = self.__upload("Procmon.exe",
                    join(OPT_DIR_UTILS, "Procmon.exe"))
            pconfig = self.__upload("Procmon.pmc",
                    join(OPT_DIR_UTILS, "Procmon.pmc"))
        else:
            raise Exception("Unknown platform: %s" % self.plat)

        # upload plugin
        plug = OPT_VM_PLUG[self.plat][tag[0:2]]
        lcur = join(OPT_DIR_SHARE, self.plat, OPT_VM_ARCH[self.plat][tag[0:2]])

        if self.plat == "mac":
            dn = "/Driver.acroplugin"
            self.__mkdir(plug + dn)

            dn = "/Driver.acroplugin/Contents"
            self.__mkdir(plug + dn)
            fn = "/Info.plist"
            self.__transfer(lcur + dn + fn, plug + dn + fn)

            dn = "/Driver.acroplugin/Contents/MacOS"
            self.__mkdir(plug + dn)
            fn = "/Driver"
            self.__transfer(lcur + dn + fn, plug + dn + fn)
            fn = "/Reader.cer"
            self.__transfer(lcur + dn + fn, plug + dn + fn) 
            fn = "/Reader.dig"
            self.__transfer(lcur + dn + fn, plug + dn + fn) 

            dn = "/Driver.acroplugin/Contents/Resources"
            self.__mkdir(plug + dn)

            dn = "/Driver.acroplugin/Contents/Resources/English.lproj"
            self.__mkdir(plug + dn)
            fn = "/InfoPlist.strings"
            self.__transfer(lcur + dn + fn, plug + dn + fn)

        elif self.plat == "win":
            fn = "Driver.api"
            self.__transfer(lcur + "/" + fn, plug + "\\" + fn)

        else:
            raise Exception("Unknown platform: %s" % self.plat)


        # get program path
        prog = OPT_VM_PROG[self.plat][tag[0:2]]

        # build script
        if self.plat == "mac":
            script = \
'''
"%s" "%s" "%s" -u %s "%s"
''' % (ptracer, self.home % OPT_FN_TRACE, 
        self.home % "sudo", OPT_VM_USER,
        prog)

        elif self.plat == "win":
            script = \
'''
start "" "%s" /accepteula /quiet /minimized /loadconfig "%s" /backingfile "%s"
"%s" /accepteula /waitforidle
start "" /wait "%s"
"%s" /terminate
"%s" /accepteula /loadconfig "%s" /openlog "%s" /saveapplyfilter /saveas "%s"
move "%s" "%s"
''' % (ptracer, pconfig, self.home % "Maldoc.pml",
        ptracer,
        prog,
        ptracer,
        ptracer, pconfig, self.home % "Maldoc.pml", self.home % "Maldoc.csv",
        self.home % "Maldoc.csv", self.home % OPT_FN_TRACE)

        else:
            raise Exception("Unknown platform: %s" % self.plat)

        return script

    def __run_script(self, script):
        cmd = [OPT_CMD_VMRUN,
                "-gu", OPT_VM_USER, "-gp", OPT_VM_PASS,
                "runScriptInGuest", self.path,
                "-activeWindow", "-interactive",
                self.bash, script]

        return exec_and_timeout(cmd, OPT_VM_LIFE)

    def __stop_script(self):
        if self.plat == "mac":
            script = \
'''
killall AdobeReader
sleep %d
''' % (OPT_VM_REST)
        elif self.plat == "win":
            script = \
'''
taskkill /f /t /im "AcroRd32.exe"
timeout %d > NUL
''' % (OPT_VM_REST)
        
        else:
            raise Exception("Unknown platform: %s" % self.plat)

        cmd = [OPT_CMD_VMRUN,
                "-gu", OPT_VM_USER, "-gp", OPT_VM_PASS,
                "runScriptInGuest", self.path,
                "-activeWindow", "-interactive",
                self.bash, script]

        exec_and_wait(cmd)

    def __download_results(self, tag, sample):
        pn = join(OPT_DIR_TRACE,
                "%s-%s-%s.out" % (self.name, tag, sample))

        self.__download(OPT_FN_TRACE, pn)

        pn = join(OPT_DIR_TRACE,
                "%s-%s-%s.log" % (self.name, tag, sample))

        self.__download(OPT_FN_LOG, pn)

    def __record(self, tag, sample):
        prec = join(OPT_DIR_VIDEO,
                "%s-%s-%s.avi" % (self.name, tag, sample))

        cmd = ["ffmpeg", "-f", "avfoundation",
                "-framerate", "10", "-video_size", "320x240",
                "-pixel_format", "bgr0",
                "-i", "1:none", "-c:v", "libx264",
                prec]

        return exec_and_interact(cmd)

    def run(self, tag, sample, force=False, record=False):
        # test if processed
        pn = join(OPT_DIR_TRACE,
                "%s-%s-%s.out" % (self.name, tag, sample))

        if exists(pn) and not force:
            return

        # download sample if needed
        self.__prepare_sample(sample)

        # start vm
        self.__launch_vm(tag)

        try:
            # prepare context
            script = self.__prepare_context(tag, sample)
            
            # record if necessary
            if record:
                ps = self.__record()
            else:
                ps = None
                
            # launch program
            if not self.__run_script(script):
                self.__stop_script()
            
            # stop recording if necessary
            if ps is not None:
                ps.send_signal(signal.SIGINT)
                sleep(1)

            # download results
            self.__download_results(tag, sample)

        except Exception as err:
            print err.message

        self.__shutdown_vm()

# main
if __name__ == "__main__":
    # machines
    machines = dict()
    machines["mac"] = Machine("x64", "mac", "11", 
            "/Users/bft/%s", "/bin/sh")
    machines["win"] = Machine("x64", "win", "7",
            "C:\\Users\\bft\\%s", "")

    # parser
    parser = ArgumentParser()
    parser.add_argument("machine", help="Machine",
            choices=("mac", "win"))
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
    parser.add_argument("-r", "--record", action="store_true")

    args = parser.parse_args()

    # run
    machine = machines[args.machine]
    try:
        machine.run(args.tag, args.sample, args.force, args.record)
    except Exception as err:
        print err.message
