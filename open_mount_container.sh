#!/bin/sh

cryptsetup luksOpen /home/$1/container/$2 $2Handle<<EOF
lol
EOF
if [ ! -d "/home/$1/container/mounted" ]
then
    mkdir /home/$1/container/mounted
fi
mount /dev/mapper/$2Handle /home/$1/container/mounted/$2