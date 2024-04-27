
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/select.h>
#define BUFFER_SIZE 256
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
    int client_socket[FD_SETSIZE];
    int num_clients = 0;
   

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

                        int j = 0 ;
                        for (; j<  num_clients ; j++) {
                            if (client_socket[j]==client) break;
                        }
                        if (j == num_clients) {
                            //chua dang nhap 
                            char user[32], pass[32], tmp[64], line[64];
                            
                            int n = sscanf(buf, "%s %s %s", user, pass, tmp);
                            printf("%s \n%s \n %s", user, pass, tmp);
                            if (n!=2){
                                char *msg = "Sai cu phap. Hay nhap lai.\n";
                                send(client, msg, strlen(msg), 0);
                            }
                            else {
                                //kiem tra thonfg tin dang nhap
                                sprintf(tmp, "%s %s\n", user, pass);
                                
                                FILE *f = fopen("account.txt", "rb");
                                int found = 0;
                                while (fgets(line, sizeof(line), f)!= NULL){
                                    
                                    if (strcmp(tmp, line)==0){
                                        found = 1;
                                        break;
                                    }
                                }
                                if (found ) {
                                    char *msg ="Dang nhap thanh cong.\n";
                                    send(client, msg, strlen(msg), 0);
                                    
                                }
                                else {
                                    char *msg= "TK ko ton tai. Thu lai.\n";
                                    send(client, msg, strlen(msg), 0);
                                }
                                fclose(f);
                            }

                        }
                        else {
                            //da dang nhap
                            char cmd[64];

                            //thuc hien ;emj 
                            if(buf[strlen(buf)-1] == '\n' ){
                                buf[strlen(buf)-1] = '0';
                            }

                            sprintf(cmd, "%s > out.txt", buf);
                            system(cmd );

                            //tra loi ket qua cjp client
                            //gửi cho client đó kết quả
                             //doc du lieu tu file de gui di
                            FILE *f = fopen("out.txt", "rb");
                            if (f == NULL){
                                printf("Cannot open file.\n");
                                exit(1);
                            }
                            char buf[BUFFER_SIZE];

                            //truyen noi dung file den client
                            fseek(f,0 , SEEK_SET);
                            while (1) {
                                int ret = fread(buf, 1 ,sizeof(buf), f);
                                if (ret <=0) break;
                                //neu trong lan doc cuoi cung so ky tu con lai < BUFFER_SIZE
                                if(ret < BUFFER_SIZE) buf[ret] = '\0';

                               
                                send(client, buf, ret,0);
                            }
                            printf("Sent successfully!");
                            fclose(f);
                        }
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