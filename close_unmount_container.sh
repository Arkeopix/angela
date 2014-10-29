#!/bin/sh

if [ $# -ne 2 ]
then
    echo "$0 username container_name"
    exit 0
fi
umount /home/$1/container/mounted/$2
cryptsetup luksClose $2Handle