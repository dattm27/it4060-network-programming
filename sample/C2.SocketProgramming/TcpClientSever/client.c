#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

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

    //gui tin nhan den server
    char *msg = "Hello server";
    send(client, msg, strlen(msg), 0);

    //nhan tin nhan tu server
    char buf[2048];
    int len = recv(client, buf, sizeof(buf), 0);
    buf[len] = 0;
    printf("Data received: %s\n", buf);

    //ket thuc , dong socket
    close(client);
    return 0;
}