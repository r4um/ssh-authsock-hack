# ssh-authsock-hack

LD_PRELOAD Hack to try and make latest ssh-agent auth socket always available.
For all those screen and ssh agent forwarding problems.

To build on Linux
    
    $ gcc -Wall -shared -fPIC -ldl -o ssh-authsock-hack.so ssh-authsock-hack.c

To build on FreeBSD

    $ gcc -Wall -shared -fPIC -o ssh-authsock-hack.so ssh-authsock-hack.c

Set LD_PRELOAD while shell initialization

For ex. bash

    if [ -e "/path/to/ssh-authsock-hack.so"]; then
        export LD_PRELOAD="/path/to/ssh-authsock-hack.so":$LD_PRELOAD
    fi
