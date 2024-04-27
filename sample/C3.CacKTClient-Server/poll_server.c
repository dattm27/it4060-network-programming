
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/poll.h>
#define MAX_FDS 2048
int main () {
       //tao socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener != -1)
        printf("Socket created: %d\n", listener);
    else {
        printf("Failed to create socket.\n");
        exit(1);
    }

    // Khai bao cau truc dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    // Gan dia chi voi socket
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))){
        printf("bind() failed.\n ");
        exit (1);
    }

    if (listen(listener,5)){
        printf("listen() failed.\n");
        exit (1);
    }
    else printf("Waiting for a new client...\n");

    //khoi tao 
    struct pollfd fds[MAX_FDS];
    int nfds =1;

    
    return 0;
}