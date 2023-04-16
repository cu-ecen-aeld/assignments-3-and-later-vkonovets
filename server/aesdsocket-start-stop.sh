#! /bin/bash

case "$1" in
    start)
        echo "Starting aesdsocket"
        start-stop-daemon -S -n aesdsocket -a /home/samokhvalov/Documents/Coursera/linux_system_programming/assignment-5-vkonovets-part1/server
        ;;
    stop)
        echo "Stopping aesdsocket"
        start-stop-daemon -K -n aesdsocket
        ;;
    *)
        echo "Usage: $0 {start|stop}"
    exit 1
esac

exit 0
