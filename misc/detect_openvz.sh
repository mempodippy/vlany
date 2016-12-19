#!/bin/sh
OVZ=0
uname -r|grep 'stab' && OVZ=$(($OVZ+1))
[ -d /proc/vz ] && OVZ=$(($OVZ+1))
if [ $OVZ != 0 ] ; then echo "We are in OpenVZ."; fi
