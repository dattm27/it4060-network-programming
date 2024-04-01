#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h> 
#include<arpa/inet.h>
#include<unistd.h>
struct SV{
    char MSSV[10];
    char ho_ten[50];
    char ngay_sinh[11];
    double diem_tb; 
};
int main (int argc, char * argv[]){
    
    //tao socket 
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //khai bao cau truc dia chi cua server muon ket noi
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    //ket noi den server
    int ret = connect(client, (struct sockaddr_in*)&addr, sizeof(addr));
    if (ret == -1 ){
        printf("connect () failed.\n");
        exit(1);
    }

    //nhap thong tin sv
    struct SV sv;
    while (1){
        printf("MSSV: ");
        scanf("%s", sv.MSSV);
        // Xóa ký tự Enter còn tồn đọng trong luồng đầu vào
        while (getchar() != '\n');
        printf("Ho va ten: ");
        fgets(sv.ho_ten, sizeof(sv.ho_ten), stdin);
        sv.ho_ten[strlen(sv.ho_ten) - 1] = '\0';
        printf("Ngay sinh (yyyy-mm-dd): ");
        scanf("%s", sv.ngay_sinh);
        printf("Diem TB: ");
        scanf("%lf", &sv.diem_tb);

        int ret = send(client, &sv, sizeof(sv), 0);
        printf("%d bytes sent \n" , ret);
      
    }

    close(client);
    

    return 0;
}