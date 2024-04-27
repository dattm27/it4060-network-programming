
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/poll.h>

int main (){

    //khai bao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //khai bao dia chi cua server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    //ket noi den sever
    int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1){
        printf("Khong ket noi duoc den server");
        return 1;
    }

    struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO; // mo ta cua thiet bi nhap du lieu
    fds[0].events = POLLIN;
    fds[1].fd = client;     // mo ta cua socket client
    fds[1].events = POLLIN;
    char buf[256];
    while (1){
        int ret = poll(fds,2, -1);
        if (fds[0].revents & POLLIN){ //neu co du lieu nhap tu ban phim
            fgets(buf, sizeof(buf), stdin);
            send(client, buf, strlen(buf), 0);
        }

        if (fds[1].revents & POLL_IN){ //nru co du lieu tu socket
            ret = recv(client, buf, sizeof(buf), 0); 
            if (ret <= 0) break;
            buf[ret] = 0;
            printf("Received %s \n", buf);
        }
    }

}