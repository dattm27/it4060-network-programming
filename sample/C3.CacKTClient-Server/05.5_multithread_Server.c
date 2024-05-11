/*
OK
Tạo server đa luồng, trong đó luồng chính chờ các client mới kết nối
Khi có một client kết nối, tạo luồng mới phục vụ client đó
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

void *client_proc (void *arg);
int main () {
    //khởi tạo server
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
  

    pthread_t tid;
    //nhan du lieu tu client
    char buf[256];
    while (1){
        printf("Waiting for a new client...\n");
        int client = accept(listener, NULL, NULL) ;
        printf("New client accepted: %d\n", client);
       
        pthread_create(&tid, NULL, client_proc , (void *) &client );
        pthread_detach(tid);
    }
    
    
    return 0;
}

void *client_proc(void *arg){
    int client = *(int * ) arg;
    char buf[256];

    //nhận dữ liệu từ client
    while(1){
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0) break;
        buf[ret] = 0;
        printf("Receive from client %d: %s\n", client, buf);
    }
}