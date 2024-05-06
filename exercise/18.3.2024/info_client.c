/*
Ứng dụng info_client lấy tên thư mục hiện tại, danh sách các tập tin và kích thước (trên máy client). Các dữ liệu này sau đó được đóng gói và chuyển sang info_server.
Ứng dụng info_server nhận dữ liệu từ info_client, tách các dữ liệu và in ra màn hình.
Yêu cầu kích thước dữ liệu được gửi là nhỏ nhất.
Ví dụ:
/mnt/c/Users/admin/source/baitap 
bai1.c – 12871 bytes 
bai2.c – 1437 bytes 
bai3.c – 1461 bytes 
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
int main (){
    
    //tao socket 
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //khai bao cau truc dia chi cua server muon ket noi
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    //ket noi den server
    int ret = connect(client, (struct sockaddr_in*)&addr, sizeof(addr));
    if (ret == -1 ){
        printf("connect () failed.\n");
        exit(1);
    }
    char buf[1024]; // de dong goi du lieu
    char path[256];
    int pos = 0 ;
    getcwd(path, sizeof(path)); //lay duong dan thu muc hien tai
    puts(path);

    strcpy(buf,path);
    buf[strlen(path)]='\n';
    pos += strlen(path)+1;
    DIR *d = opendir(path); //mở thư mục
    struct dirent *dir;
    struct stat st;
    if (d != NULL)  
    while ((dir = readdir(d)) != NULL) 
    {    if (dir->d_type == DT_REG) {      
            stat(dir->d_name, &st);      
            printf("%s - %ld bytes\n", dir->d_name, st.st_size);   

            strcpy(buf + pos, dir->d_name);
            buf[pos + dir->d_namlen]='*'; // thêm dấu gạch sau mỗi tên file để phân biệt
            pos+= strlen(dir->d_name) +1;
            memcpy(buf + pos, &st.st_size, sizeof(st.st_size));
            pos+= sizeof(st.st_size);
        }  
    }
  


    //gui sang server
    ret = send(client, buf,pos, 0);
    printf("%d bytes sent.\n", ret);
    close(client);
    

    return 0;
}