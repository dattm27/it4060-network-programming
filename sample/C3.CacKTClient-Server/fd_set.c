
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/select.h>

int main () {
    fd_set fdread;
    int socket1 = 65;
    int socket2 = 67;
    FD_ZERO(&fdread);
    FD_SET(socket1, &fdread);
    FD_SET(socket2, &fdread);

    return 0;
}