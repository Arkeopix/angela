#!/bin/sh

if [ $# -ne 2 ]
then
    echo "$0 username container_name"
    exit 0
fi
echo "Unmounting container $2 ($1)"
umount /home/$1/container/mounted/$2
cryptsetup luksClose $2Handle
rm -v "/tmp/pamela/$1+$2"
