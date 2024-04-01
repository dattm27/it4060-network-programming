#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h> 
#include<arpa/inet.h>
#include<unistd.h>


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

 
    //nhap du lieu tu ban phim
    int mssv;
    char hoten[64];
    unsigned char ngay, thang;
    unsigned short nam;
    float diem;

    while (1){
        printf("Nhap MSSV: ");
        scanf("%d", &mssv); 
        if (!mssv) break;
        printf("Nhap ho ten: ");
        getchar();   //xoa ky tu xuong dong khoi bo dem ban phim
        fgets(hoten, sizeof(hoten), stdin);
        hoten[strlen(hoten)-1]='\0';
        printf("Nhap ngay, thang, nam sinh: ");
        scanf("%hhu%hhu%hu", &ngay, &thang, &nam);
        printf("Nhap diem: ");
        scanf("%f", &diem);
        
        //dong goi du lieu
        char buf[256];
        int pos = 0 ;
        //de ho ten o cuoi --> de tach
        memcpy(buf, &mssv, sizeof(mssv));
        pos += sizeof(mssv);  //4 bytes
        buf[pos++] = ngay;    //1 byte
        buf[pos++] = thang;   //1 byte
        memcpy(buf + pos, &nam, sizeof(nam));
        pos += sizeof(nam);
        memcpy(buf + pos, &diem, sizeof(diem));
        pos+= sizeof(diem);
        memcpy(buf + pos, hoten, strlen(hoten) );
        pos += strlen(hoten);

        //gui sang server
        int ret = send(client, buf, pos, 0);
        printf("\n%d bytes sent.\n", ret);
    }
    
    close(client);
    

    return 0;
}