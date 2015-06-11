#!/bin/bash
set -ex

pebble build

if [[ -n "$1" ]]; then
    if [[ "$1" == "aplite" || "$1" == "basalt" ]]; then
        target="--emulator $1"
    else
        target="--phone $1"
        echo -n "Press [Enter] to install, [Ctrl-C] to exit."; read -n 1 -s;
    fi

    pebble install $target
    pebble logs $target
fi
