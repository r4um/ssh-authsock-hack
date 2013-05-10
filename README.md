# ssh-authsock-hack

LD_PRELOAD Hack to try and make latest ssh-agent auth socket always available.
For all those screen and ssh agent forwarding problems.

To build

    $ make

Set LD_PRELOAD while shell initialization

For ex. bash

    if [ -e "/path/to/ssh-authsock-hack.so"]; then
        export LD_PRELOAD="/path/to/ssh-authsock-hack.so":$LD_PRELOAD
    fi
