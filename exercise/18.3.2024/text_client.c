/*
Ứng dụng client đọc nội dung file văn bản và gửi sang server.
Ứng dụng server nhận dữ liệu từ client, in ra màn hình số lần xuất hiện xâu ký tự (server không cần tạo file để chứa nội dung nhận được): “0123456789”
Chú ý cần xử lý trường hợp khi xâu “0123456789” nằm giữa 2 lần truyền.
Ví dụ nội dung file văn bản:
SOICTSOICT0123456789012345678901234567890123456789SOICTSOICTSOICT01234567890123456789012345678901234567890123456789012345678901234567890123456789SOICTSOICT
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h> 
#include<arpa/inet.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#define BUFFER_SIZE 16
int main () {
    //tao socket cho ket noi
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //khai bao cau truc dia chia server muon ket noi
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port= htons(9000);

    //ket noi den server
    int server = connect(client, (struct sockaddr_in *)&addr,sizeof(addr));
    if (server ==-1){
        printf("connect() failed.\n");
        exit(1);
    }
    //doc du lieu tu file de gui di
    FILE *f = fopen("text.txt", "rb");
    if (f == NULL){
        printf("Cannot open file.\n");
        exit(1);
    }
    char buf[BUFFER_SIZE];

    //truyen noi dung file den server
    fseek(f,0 , SEEK_SET);
    while (1) {
        int ret = fread(buf, 1 ,sizeof(buf), f);
        if (ret <=0) break;
        //neu trong lan doc cuoi cung so ky tu con lai < BUFFER_SIZE
        if(ret < BUFFER_SIZE) buf[ret] = '\0';
        //printf("send %s\n", buf);
        send(client, buf, ret,0);
    }
    printf("Sent successfully!");
    
    
    
    fclose(f);

    close(client);

    
    return 0;
}