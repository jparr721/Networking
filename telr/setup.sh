#!/bin/bash
if [[ $EUID -ne 0 ]]; then
    echo "Please run this script as root"
    exit 1
fi

python_version = $(python -c 'import sys; print(sys.version_info[0])')
# Check python version
if [[ $python_version == "2" ]];
then
    sudo apt-get update && sudo apt-get upgrade -y
    sudo apt-get install -y python3 python3-pip
    sudo pip3 install gspread oauth2client apscheduler wifi
else
    sudo pip3 install gspread oauth2client apscheduler wifi
fi

echo "Evironment set up, you good playa"
