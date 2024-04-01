
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
    
    fd_set fdread, fdtest;
    FD_ZERO(&fdread);

    //gan socket listener vao fdread
    FD_SET(listener, &fdread);
    char buf[256];
    while(1){
        //khoi tao lai fd_set = fdread;
        fdtest = fdread;
        int ret = select(FD_SETSIZE, &fdtest, NULL, NULL, NULL);
        if (ret ==-1){
            break;
        }
        //kiem tra xem su kien cua socket nao 
        //-> kiem tra tat ca cac su kien co the co
        for (int i = 0 ; i< FD_SETSIZE ;i++){
            if(FD_ISSET(i, &fdtest)){ // socket i xay ra su kien
                if(i == listener){
                    //co ket noi -> su kien cua server
                    int client = accept(listener, NULL, NULL);
                    if( client >= FD_SETSIZE){
                        close(client);
                    }
                    else {
                        //them vao fread 
                        FD_SET(client, &fdread);
                        printf("New client connected: %d\n", client);
                    }

                }
                else {
                    //co du lieu truyen -> Nhan du liey 
                    int client = i;
                    ret = recv(client, buf, sizeof(buf), 0);
                    if ( ret<= 0){
                        FD_CLR(client, &fdread);
                        close (client);
                    }
                    else {
                        buf[ret] = 0;
                        printf("Received from %d: %s\n",client, buf );
                    }
                }
            }
        }
    }

    return 0;
}

/*
Khong toi uu bang phien ban 1 vi phai kiem tra tat ca 
*/