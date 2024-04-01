#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main (int argc, char * argv[]){
    //neu khong nhap du tham so -> thoat
    if (argc < 4) {
        printf("Missing arguments ");
        return 0;
    }

    //tao socket cho ket noi 
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1) {
        perror("socket() failed");
        return 1;
    }

    //khai bao dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    // gan socket voi cau truc dia chi
    if(bind(listener, (struct sockaddr_in *)&addr, sizeof(addr))){
        perror("bind () failed");
        return 1;
    }

    //Chuyen socket sang trang thai cho ket noi
    if(listen(listener, 5)){
        perror("listen () failed");
        return 1;
    }
    printf("Waiting for connection...\n");
    
    int client = accept(listener, NULL, NULL);

    // doc file chua xau chao
    FILE *f = fopen(argv[2], "rb");
    char buf[2048];
    
    //truyen noi dung file den client
    fseek(f, 0, SEEK_SET);
    while(1){
        int ret = fread(buf, 1, sizeof(buf), f);
        if (ret <= 0) break;
        send(client, buf, ret, 0);
    }
    fclose(f);

    // nhan noi dung tu client gui den
    f = fopen(argv[3], "wb"); //mo file co ten tu tham so dong lenh
    while (1){
        int  ret = recv(client,buf, sizeof(buf), 0);
        if (ret <= 0){
            break;

        }
        //ghi ra file
        fwrite(buf, 1, ret, f);
    }
    fclose(f);
   
    close(client);
    close(listener);
    return 0;
}