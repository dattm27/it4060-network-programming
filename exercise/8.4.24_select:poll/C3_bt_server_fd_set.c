
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<ctype.h>
int remove_client(int client, int *client_sockets, char **client_names, int *num_clients){
    //xoa socket , xoa ten, num_clients giam di 1
    int i = 0;
    for (; i < *num_clients ; i++){
        if(client_sockets[i]==client) break;
    }
    if(i < *num_clients){
        if(i < *num_clients-1) {
            //gan thong tin phan tu cuoi = phan tu tai i
            client_sockets[i] = client_sockets[*num_clients -1];
            strcpy(client_names[i],client_names[*num_clients -1]);
            

        }
        //giai phong bo nho
        free(client_names[*num_clients-1]);
        *num_clients-= 1;

    }
}

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
    //mang luu tru cac client da dang nhap
    int client_socket[FD_SETSIZE];
    int num_clients = 0;
    char *client_names[FD_SETSIZE]; // moi ten 50 ky tu
    int num_connections = 0;
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

                        //gui lui chao kem so luong clients hien tai
                        char *welcome = "Xin chao. Hien dang co ";
                        char *clients ="client dang ket noi.\n";
                        char msg[256];
                        sprintf(msg, "%s %d %s", welcome, ++ num_connections , clients);
                        send(client, msg, strlen(msg), 0);
                    }

                }
                else {
                    //co du lieu truyen -> Nhan du liey 
                    int client = i;
                    ret = recv(client, buf, sizeof(buf), 0);
                    if ( ret<= 0){
                        FD_CLR(client, &fdread);
                        close (client);
                        num_connections--;
                        //xoa client ra khoi mang dang nhap
                        //xoa id (ten) va xoa socket
                        remove_client(client, client_socket, client_names, &num_clients);
                    }
                    else {
                        buf[ret] = 0;
                        printf("Received from %d: %s\n",client, buf );
                        //ma hoa du lieu
                        for(int k = 0 ; k < strlen(buf) ; k++){
                            if (isalpha(buf[k])){
                               
                                if (buf[k] =='z') buf[k] = 'a';
                                else {
                                    if (buf[k]=='Z')  buf[k]= 'A';
                                    else  buf[k]= buf[k]++;
                                }
                                
                            }
                            if ( isdigit(buf[k])){
                                buf[k]= ('9'- (buf[k]-'0'));
                            }
                        }
                        send( client, buf, strlen(buf), 0);
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