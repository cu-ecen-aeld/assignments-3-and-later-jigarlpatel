#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 {start|stop}"
    exit 1
fi

if [ "$1" = "start" ]; then
    echo "Starting aesdsocket"
    start-stop-daemon -S -n aesdsocket -a /usr/bin/aesdsocket -- -d
elif [ "$1" = "stop" ]; then
    echo "Stopping aesdsocket"
    start-stop-daemon -K -n aesdsocket
else
    echo "Usage: $0 {start|stop}"
    exit 1
fi
