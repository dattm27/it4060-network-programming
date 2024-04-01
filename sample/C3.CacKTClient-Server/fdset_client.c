
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

    fd_set fdread;
    FD_ZERO(&fdread);
    char buf[256];
    while (1)
    {
        //gan du lieu vao tap fdread
        FD_SET(STDIN_FILENO, &fdread);
        FD_SET(client, &fdread);
       
        int maxdp = client + 1;
        int ret = select (client +1 , &fdread, NULL, NULL, NULL);
        if (ret == -1 ){
            printf("select() failed");
            break;
        }
        //kiem tra kieu su kien 
        if (FD_ISSET(STDIN_FILENO, &fdread))// kiem tra  nhap du ;ieu tu ban phim
        {
            //Doc du lieu tu ban phim, gui sang server
            fgets(buf, sizeof(buf), stdin);
            ret = send(client, buf, sizeof(buf),0);
            printf("%d bytes sent.\n", ret);
            if (strcmp(buf, "exit") ==0) break;

        }
        if (FD_ISSET(client, &fdread)) { //kiem tra socket client
            ret = recv(client, buf, sizeof(buf), 0);
            buf[ret]  =0;
            printf("Received: %s\n",buf);

        }
    }
    close (client);
    return 0;
}