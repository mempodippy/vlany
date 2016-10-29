#!/usr/bin/env python

# usage:
# ./detect_proc_forge.py
# optional argument: specific proc id, example:
# ./detect_proc_forge.py 1337

import sys
import os

PROC_PATH = "/proc/{0}/"
MAPS_PATH = "/proc/{0}/maps"
SMAPS_PATH = "/proc/{0}/smaps"
MAP_FILES_PATH = "/proc/{0}/map_files/"

SMAPS_BLACKLIST = ["Size", "Rss", "Pss", "Shared_Clean", "Shared_Dirty", "Private_Clean", "Private_Dirty",
                   "Referenced", "Anonymous", "AnonHugePages", "Shared_Hugetlb", "Private_Hugetlb", "Swap",
                   "SwapPss", "KernelPageSize", "MMUPageSize", "Locked", "VmFlags"]

def get_map_files(proc_id):
    try:
        contents = os.listdir(MAP_FILES_PATH.format(proc_id))
    except OSError:
        print("{0} is not present in the procfs.".format(MAP_FILES_PATH.format(proc_id)))
        quit()

    maps = ""

    for x in contents[3:]:
        maps = maps + x + "\n"

    return maps

def read_maps(proc_id):
    fd = open(MAPS_PATH.format(proc_id), "r")
    ret = fd.read()
    fd.close()
    return ret

def read_smaps(proc_id):
    fd = open(SMAPS_PATH.format(proc_id), "r")
    ret = fd.read()
    fd.close()
    return ret

def getaddrs_maps(proc_maps):
    proc_maps = proc_maps.split("\n")
    addrs = ""

    for x in proc_maps:
        if not x.split(" ")[0].startswith("0") and not x.split(" ")[0].startswith("f"):
            addrs = addrs + x.split(" ")[0] + "\n"

    return addrs[:-2]

def getaddrs_smaps(proc_smaps):
    proc_smaps = proc_smaps.split("\n")
    addrs = ""

    for x in proc_smaps:
        if not x.split(" ")[0][:-1] in SMAPS_BLACKLIST and not x.split(" ")[0].startswith("0") and not x.split(" ")[0].startswith("f"):
            addrs = addrs + x.split(" ")[0] + "\n"

    return addrs[:-2]
        

def compare_maps(proc_id, maps_addrs, smaps_addrs, map_files):
    maps_addrs = maps_addrs.split("\n")
    smaps_addrs = smaps_addrs.split("\n")
    map_files = map_files.split("\n")

    for x in maps_addrs:
        if not x in map_files and not x in smaps_addrs:
            print("\033[33mAddress {0} is not in /proc/{1}/map_files/, possible attempt to hide a malicious shared library.\033[0m".format(x, proc_id))

def is_valid_proc(proc_id):
    if os.path.isdir(PROC_PATH.format(proc_id)):
        return True
    else:
        return False

def main(proc_id):
    if not proc_id == "self":
        try:
            proc_id = int(float(proc_id))
        except ValueError:
            print("Invalid process ID. Exiting.")
            quit()

    if not is_valid_proc(proc_id):
        print("Process ID {0} does not exist.".format(proc_id))
        quit()

    # print("\nAddresses:")
    
    maps = read_maps(proc_id)
    maps_addrs = getaddrs_maps(maps)
    # print(maps_addrs)

    smaps = read_smaps(proc_id)
    smaps_addrs = getaddrs_smaps(smaps)
    # print(smaps_addrs)

    # print("\nContents of map_files:")
    map_files = get_map_files(proc_id)
    # print(map_files)

    compare_maps(proc_id, maps_addrs, smaps_addrs, map_files)

    print("\n{0} has finished checking for inconsistencies in {1} relative to {2} in process '{3}'.".format(sys.argv[0], MAPS_PATH.format(proc_id), MAP_FILES_PATH.format(proc_id), proc_id))
    print("If you see no warnings, then you're either: not infected, you are but the malware doesn't touch these files, or you are but the malware is very well hidden.")

if __name__ == "__main__":
    proc_id = ""
    if len(sys.argv) == 1:
        print("Assuming proc_id as self")
        proc_id = "self"
    else:
        print("Checking process ID {0}".format(sys.argv[1]))
        proc_id = sys.argv[1]
    main(proc_id)
