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
cryptsetup luksOpen /home/$1/container/$2 $2Handle
echo "applying ext4"
mkfs.ext4 -j /dev/mapper/$2Handle
echo "closing containner"
cryptsetup luksClose $2Handle

echo "tell me, what was the password did you enter ?"
read passwd

#if [ ! -e /etc/ctpasswd ]
#then
#    echo "touching /etc/ctpasswd"
#    touch /etc/ctpasswd
#fi

#enc_pass=$( openssl passwd -1 $passwd )
#echo "$1:$enc_pass" >> /etc/ctpasswd

echo "i'm going to mail it to $1"

echo "
hi $1 !

this is an automatic mail from your administrator.
You receive it because you were deemed worthy of handling your own 
secure container.

here is your container name: $2
and here is the password which you'll need to do stuff to it: $passwd

go and update your ~/.container file

Have fun =)
" | mail -s "New container" $1

echo "all done. see you soon =)"
