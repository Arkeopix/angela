#!/bin/sh

echo "session		optional	pamela.so" >> /etc/pam.d/login
echo "making shared library"
make
echo "copying lib to /lib/security"
cp pamela.so /lib/security