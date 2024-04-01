#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>
struct SV{
    char MSSV[10];
    char ho_ten[50];
    char ngay_sinh[11];
    double diem_tb; 
};
int main (int argc, char * argv[]){
    if( argc < 3){
        printf("Missing arguments");
        exit (1);
    }
    //tao socket cho ket noi
    
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1){
        perror("socket() failed");
        return 1;
    }
    
    // khai bao dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    //gan socket voi cau truc dia chi
    if (bind(listener, (struct sockaddr_in*) &addr,sizeof(addr))){
        perror("bind () failed");
        return 1;
    }

    //Chuyen socket sang trang thai cho ket noi
    if (listen(listener, 5)){
        perror ("listen() failed");
        return 1;
    }
    printf("Waiting for connection...\n");
    
    //Dia chi client dang ket noi 
    struct sockaddr_in clientAddr;
    unsigned int clientAddrLen = sizeof(clientAddr);
    
    int client = accept(listener,(struct sockaddr *) &clientAddr, &clientAddrLen);
    printf("Client connected: %d\n", client);
    //nhap thong tin sinh vien va ghi ra file log
    FILE * f = fopen(argv[2], "wb");
    struct SV sv;
    while (1){
        int ret = recv(client, &sv, sizeof(sv),0);
        if (ret <=0) break;
        
        //lay ra ngay gio ghi 
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        char log_date[20];
        strftime(log_date, sizeof(log_date), "%Y-%m-%d %H:%M:%S", timeinfo);
        //ghi ra file
        fprintf(f, "%s %s %s %s %s %.2lf\n",inet_ntoa(clientAddr.sin_addr), log_date, sv.MSSV, sv.ho_ten, sv.ngay_sinh, sv.diem_tb);

    }
    fclose(f);
    close(client);
    close (listener);
    return 0;

}