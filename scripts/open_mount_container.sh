#!/bin/sh

if [ $# -ne 2 ]
then
    echo "$0 username container_name"
    exit 1
fi

if [ ! -d "/home/$1/container/mounted" ]
then
    echo "Creating mounted directory"
    mkdir /home/$1/container/mounted
    chown -R $1 /home/$1/container/mounted
fi

echo "opening container $2"
cryptsetup luksOpen /home/$1/container/$2 $2Handle

if [ ! -d "/home/$1/container/mounted/$2" ]
then
    echo "Creating container ($2) directory"
    mkdir /home/$1/container/mounted/$2
    chown -R $1 /home/$1/container/mounted/$2
fi
if [ ! -d "/tmp/pamela" ]
then
    mkdir /tmp/pamela
fi

mount /dev/mapper/$2Handle /home/$1/container/mounted/$2
chown $1 /home/$1/container/mounted/$2
touch "/tmp/pamela/$1+$2"
