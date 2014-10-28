#!/usr/bin/sh

if [ $# -eq 0 ]
then
    echo "$0 username"
    exit 1
fi

dd if=/dev/zero of=/home/$1/container bs=1M count=512
cryptsetup -y luksFormat /home/$1/container