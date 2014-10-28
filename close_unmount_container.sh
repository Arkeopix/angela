#!/bin/sh

umount /home/$1/container/mounted/$2
cryptsetup luksClose $2Handle