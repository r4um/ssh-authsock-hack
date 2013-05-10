UNAME := $(shell uname -s)

DetectOS:
	-@make $(UNAME)

Linux:
	gcc -Wall -shared -fPIC -o ssh-authsock-hack.so ssh-authsock-hack.c -ldl

FreeBSD:
	gcc -Wall -shared -fPIC -o ssh-authsock-hack.so ssh-authsock-hack.c

clean:
	rm *.so
