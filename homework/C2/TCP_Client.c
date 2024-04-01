#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h> 
#include<arpa/inet.h>
#include<unistd.h>

int main (int argc, char *argv []){
    //neu khong nhap du tham so -> exit
    if (argc < 3 ) {
        printf("IP address or Port missing. Exiting...\n");
        return 0;
    }

    //khai bao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //khai báo địa chỉ của server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int port = atoi(argv[2]) ;
    addr.sin_port = htons(port);

    //ket noi den server
    int res = connect (client, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1){
        printf("Connection to %s port %d failed!", argv[1], port);
        return 0;
    }

    //nhan thong diep tu server gui den
    char buf[1024];
    int ret = recv(client, buf, sizeof(buf), 0);    
    if (ret < sizeof(buf))
        buf[ret] = 0;
    puts(buf);
    

    //gui thong diep nhap tu ban phim den server
    char msg[5000];
    while (1){
        printf("Type messsage: ");
        fgets(msg, sizeof(msg), stdin);
        int ret =  send(client, msg, strlen(msg),0);
        if(strncmp(msg, "exit",4) == 0) break;
        printf("%d bytes sent succesfully!\n", ret);

    }
   
    close(client); //đóng kết nối
    return 0;
}