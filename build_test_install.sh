#!/bin/bash
set -ex
start_emulator=false

while [[ -n "$1" && "${1:0:1}" == "-" ]]; do
  if [[ "$1" == "-s" ]]; then
    start_emulator=true
  fi
  shift
done

pebble build
if [[ -n "$1" ]]; then
    if [[ "$1" == "aplite" || "$1" == "basalt" ]]; then
        target="--emulator $1"
        pebble rm $target 0 || true
    else
        target="--phone $1"
        echo -n "Press [Enter] to install, [Ctrl-C] to exit."; read -n 1 -s;
    fi
    if [[ "$start_emulator" == "true" ]]; then
      nohup pebble install $target &
      sleep 3
    fi
        
    pebble install $target
    pebble logs $target
fi
