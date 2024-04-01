#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main (){
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1){
        perror ("socket() failed");
        return 1;
    }

    //khai bao cau truc dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);
    
    //gan socket voi cau truc dia chi
    if (bind(listener, (struct sockaddr_in*)&addr, sizeof(addr))){
        perror("bind() failed");
        return 1;
    }
    //chuyen socket sang trang thai cho ket noi
     
    if(listen(listener,5)){
        perror("listen() failed");
        return 1;
    }
  else  printf("Waiting for connection...\n");

    //chap nhan ket noi cua client
    int client = accept(listener, NULL, NULL);
    printf("Client connected: %d\n", client);

    char buf[1024];
  
    int ret = recv(client, buf, sizeof(buf), 0);
    int pos = 0;
    while (pos < ret){
        if (buf[pos] =='*'){
            long filesize;
            memcpy(&filesize, buf + pos +1, sizeof(long));
            printf(" - %ld bytes \n", filesize);
            pos+= sizeof(long); //tăng đúng bằng kích thước của long để đến tên file tiép theo
        }
        else putchar(buf[pos]);
        pos ++;
    }

    
    close(client);
    close (listener);
    

    return 0;
}