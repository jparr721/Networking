#!/bin/bash
function usage() {
    echo "Usage: ./pinger.sh [network]"
    echo "Example: ./pinger.sh 192.168.2"
}
if [ "$1" == "" ]
then
usage
else
    for x in `seq 1 254`; do
        ping -c 1 $1.$x | grep "64 bytes" | cut -d" " -f4 | sed 's/.$//'
    done
fi
