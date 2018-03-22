#!/bin/bash
if [[ $EUID -ne 0 ]]; then
    echo "Please run this script as root"
    exit 1
fi

python_version = $(python -c 'import sys; print(sys.version_info[0])')
# Check python version
if [[ $python_version == "2" ]];
then
    sudo yum -y update
    sudo yum -y install https://centos7.iuscommunity.org/ius-release.rpm 
    sudo yum -y install python36u
    sudo yum -y install python36u-pip
    sudo pip3 install gspread oauth2client
else
    echo "Evironment set up, you good playa"
fi
