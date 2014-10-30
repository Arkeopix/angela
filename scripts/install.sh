#!/bin/sh

echo "making shared library"
make
echo "copying lib to /lib/security"
cp src/pamela.so /lib/security

if [ ! -d "/user/share/pamela" ]
then
    echo "Creating scripts installation folder"
    mkdir /usr/share/pamela
    echo "Copying scripts.."
    cp ./scripts/close_unmount_container.sh /usr/share/pamela
    cp ./scripts/open_mount_container.sh /usr/share/pamela
    cp ./scripts/install.sh /usr/share/pamela
    cp ./scripts/create_container.sh /usr/share/pamela
    echo "Done!"
fi

string="session optional pamela.so"

if ! grep -xqx "$string" /etc/pam.d/login
then
    echo $string >> /etc/pam.d/login
fi
