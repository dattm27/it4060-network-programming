
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
    
    fd_set fdread;
    int clients[FD_SETSIZE];
    int numClients = 0 ;
    int maxdp;
    char buf[256];
    while (1){
        //khoi tao fdread
        FD_ZERO(&fdread);

        //gan cac socket vao tap fdread
        FD_SET(listener, &fdread);
        maxdp = listener;
        for (int i = 0 ; i < numClients; i++){
            FD_SET(clients[i], & fdread);
            if (clients[i] + 1 > maxdp) maxdp = clients[i] + 1;

        }

        //goi ham cho su kien
        int ret = select(maxdp  + 1, &fdread , NULL, NULL, NULL);
        if(ret == -1){
            break;
        }

        //kien  tra su kien vua socket nao
        if (FD_ISSET(listener, &fdread)){
            // co ket noi moi
            int client = accept(listener, NULL, NULL);
            if (client >= FD_SETSIZE){
                close(client);
            }
            else {
                //luu socket vao trong mang
                clients[numClients] = client;
                numClients ++;
                printf("New client connected %d\n", client);

            }
        }

        for(int i = 0 ; i< numClients ; i++){
            if(FD_ISSET(clients[i], &fdread)){
                ret = recv(clients[i], buf, sizeof(buf), 0);
                if (ret  <= 0){
                    //ket noi bi ngat
                    //xoa phan tu khoi mang tai vi tri i
                    //doi choi phan tu i voi phan tu cuoi cung
                    if ( i < numClients ){
                        clients[i] = clients[numClients - 1];
                    }
                    numClients --;
                    i--;
                    printf("Connection closed: %d\n", clients[i]);
             
                    
                    continue;
                  
                }   

                else {
                    // xu ly du lieu nhan duoc
                    buf[ret] = 0 ;
                    printf("Received from %d: %s \n", clients[i], buf);
                }
            }
        }

    }
    return 0;
}