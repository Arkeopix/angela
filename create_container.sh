#!/bin/sh

if [ $# -ne 3 ]
then
    echo "$0 username container_name part_size"
    exit 1
fi

if [ ! -d "/home/$1/container" ]
then
    echo "Creating /home/$1/container/$2"
    mkdir /home/$1/container
fi

echo "creating partition in /home/$1/container/$2"
dd if=/dev/zero of=/home/$1/container/$2 bs=1M count=$3
echo "formating with luksFormat"
cryptsetup -y luksFormat /home/$1/container/$2
echo "opening container"
cryptsetup luksOpen /home/$1/container/$2 $3_handle
echo "applying ext4"
mkfs.ext4 -j /dev/mapper/$3_handle
echo "closing containner"
cryptsetup luksClose $3_handle