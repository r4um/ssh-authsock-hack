/*
* LD_PRELOAD Hack to try and make latest ssh-agent auth socket
* always available. Works on Linux/FreeBSD
*
* To build on Linux
*  $ gcc -Wall -shared -fPIC -ldl -o ssh-authsock-hack.so ssh-authsock-hack.c
* Tom build on FreeBSD
*  $ gcc -Wall -shared -fPIC -ldl -o ssh-authsock-hack.so ssh-authsock-hack.c
*
* Set LD_PRELOAD while your shell initialization
*
* For ex. bash
*
* if [ -e "/path/to/ssh-authsock-hack.so"]; then
* export LD_PRELOAD="/path/to/ssh-authsock-hack.so":$LD_PRELOAD
* fi
*/

#define _GNU_SOURCE

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <glob.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#if defined(RTLD_NEXT)
    #define REAL_LIBC RTLD_NEXT
#else
    #define REAL_LIBC ((void *) -1L)
#endif

time_t path_mtime(const char *path)
{
     struct stat s;
     time_t mtime = 0;
     int ret;

     ret = stat(path, &s);
     if(ret == 0) {
        mtime = s.st_mtime;
     }
     return mtime;
}

static int cmp_mtime(const void *p1, const void *p2)
{
       int ret = 0;

       time_t m1 = path_mtime(* (char * const *) p1);
       time_t m2 = path_mtime(* (char * const *) p2);

       if( m1 > m2)
           ret = 1;

       if( m1 < m2)
           ret = -1;

       return ret;
}

char * latest_ssh_auth_sock(void)
{
    glob_t g;
    char *sock;
    char *latest;
    size_t latest_length;


    /* don't set GLOB_ERR or GLOB_DOOFFS */
    if(glob("/tmp/ssh-*/agent*", GLOB_NOSORT, NULL, &g) == 0) {
        qsort(&g.gl_pathv[0], g.gl_pathc, sizeof(g.gl_pathv[0]), cmp_mtime);

        latest = g.gl_pathv[g.gl_pathc - 1];
        latest_length = strlen(latest);

        sock = (char *) malloc( (latest_length + 1) * sizeof(char) );
        sock = strncpy(sock, latest, latest_length);
        sock[latest_length] = '\0';
    } else {
        sock = NULL;
    }

    globfree(&g);

    return sock;
}

char *getenv(const char *name)
{
   static char* (*o_dl)(const char *name) = 0;
   char *new_sock;

   o_dl = (char*(*)(const char *name)) dlsym(REAL_LIBC,"getenv");

   if(strcmp(name, "SSH_AUTH_SOCK") == 0) {
       new_sock = latest_ssh_auth_sock();

       if(new_sock != NULL) {
           /*printf("ssh-agent-hack.so: returning %s for SSH_AUTH_SOCK\n", new_sock);*/
           return new_sock;
       }
   }

   return (*o_dl)(name);
}

