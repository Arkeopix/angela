#!/bin/sh

echo "making shared library"
make
echo "copying lib to /lib/security"
cp src/pamela.so /lib/security

string="session optional pamela.so"

if ! grep -xqx "$string" /etc/pam.d/login
then
    echo $string >> /etc/pam.d/login
fi