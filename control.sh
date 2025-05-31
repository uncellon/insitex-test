#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${0}")" && pwd)"
SERVER_EXECUTABLE="${SCRIPT_DIR}/build/bin/server/InsitexServer"
CLIENT_EXECUTABLE="${SCRIPT_DIR}/build/bin/client/InsitexClient"

printHelp() {
    echo "Usage: $(basename "$0") [start|stop|restart]"
    echo ""
    echo "Start/stop script for Insitex executables"
    echo ""
    echo "Options:"
    echo "  start                              start server and client"
    echo "  stop                               stop server and client"
    echo "  restart                            stop and start server and client"
    echo ""
    echo "Examples:"
    echo "  $(basename "$0") start"
    echo "  $(basename "$0") stop"
    echo "  $(basename "$0") restart"
}

start() {
    pgrep InsitexServer >/dev/null 2>&1
    SERVER_STARTED=$?
    pgrep InsitexClient >/dev/null 2>&1
    CLIENT_STARTED=$?

    if [ $SERVER_STARTED -eq 0 ] || [ $CLIENT_STARTED -eq 0 ]; then
        echo "Server and/or client already started!"
        exit 1
    fi

    cd "$(dirname "${SERVER_EXECUTABLE}")" && $SERVER_EXECUTABLE >/dev/null 2>&1 &
    sleep 1

    # There is an issue here where the 'client' application is waiting for
    # input via getline(...). And since it's being launched in the background,
    # the client doesn't receive a valid input descriptor, which causes 100%
    # CPU usage
    $CLIENT_EXECUTABLE >/dev/null 2>&1 &

    echo "Server and client started"
}

stop() {
    pkill InsitexClient >/dev/null 2>&1
    kill -SIGINT $(pgrep InsitexServer) >/dev/null 2>&1
    echo "Server and client stopped"
}

case "$1" in
    start)
        start
        ;;
    stop)
        stop
        ;;
    restart)
        stop
        start
        ;;
    *)
        printHelp
        ;;
esac
