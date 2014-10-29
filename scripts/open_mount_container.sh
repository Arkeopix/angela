#!/bin/sh

if [ $# -ne 2 ]
then
    echo "$0 username container_name"
    exit 1
fi

if [ ! -d "/home/$1/container/mounted" ]
then
    mkdir /home/$1/container/mounted
fi

cryptsetup luksOpen /home/$1/container/$2 $2Handle<<EOF
lol
EOF
if [ ! -d "/home/$1/container/mounted/$2" ]
then
    mkdir /home/$1/container/mounted/$2
fi
mount /dev/mapper/$2Handle /home/$1/container/mounted/$2