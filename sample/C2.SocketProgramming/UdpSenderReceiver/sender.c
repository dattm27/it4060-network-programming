#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int main () {
    // tao socket theo giao UDP
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    char *msg = "Hello. Iam seeding a message.\n";

    //khai bao dia chi nhan
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr= inet_addr("127.0.0.1");
    addr.sin_port = htons(9090);

    //gui tin nhan
    char buf[256];
    while (1){
        printf("Enter Message: ");
        fgets(buf, sizeof(buf), stdin);
        sendto(sender, buf, strlen(buf),0, (struct sockaddr *) &addr, sizeof(addr));

    }
    return 0;
}
