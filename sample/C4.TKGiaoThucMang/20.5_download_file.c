/*
OK
c. Download file từ Internet
http://lebavui.github.io/network_programming/videos/ecard.m p4
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


int main()
{

    // khai bao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //phan giai ten mien 
    struct addrinfo *info;
    int ret = getaddrinfo("storage.googleapis.com", "http", NULL, &info);
    if (ret != 0) {
        printf("Khong phan giai duoc");
        return 1;
    }
   

    // ket noi den sever
    int res = connect(client, info->ai_addr, info->ai_addrlen);
    if (res == -1)
    {
        printf("Khong ket noi duoc den server");
        return 1;
    }

    char req[] = "GET /it4060_samples/ecard.mp4 HTTP/1.1\r\nHost:storage.googleapis.com\r\nConnection: close\r\n\r\n";
   // char req2[] = "POST /post HTTP/1.1\r\nHost:httpbin.org\r\nContent-Length: 27\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nparam1=value1&param2=value2";
    send(client, req, strlen(req), 0);
  
    char buf[2048];
    //nhận nội dung đến phần header thì dừng lại
    char *header = NULL;
    int len =0; 
    
    
    while (1){
        int ret = recv(client, buf, sizeof(buf) -1, 0);
        if (ret <= 0){
            close(client);
            printf("Disconnected");
            return 1;
        }
        
        //tái cấp phát bộ nhớ cho vùng header
        header = realloc(header, len+ ret);
        
        //copy tiếp phần mới nhận được vào nối vào header
        memcpy(header + len, buf, ret);
        len += ret ;

        if (memmem(header, len, "\r\n\r\n", 4) != NULL){
            break;
        }

        
    }
    //In header
    puts(header);
    char *pos = memmem(header,len, "\r\n\r\n", 4);

    FILE *f = fopen("download.mp4", "wb");
   

  
    //nhận nội dung file 
    int header_length = pos - header; //độ dài phần header
    if (header_length + 4 < len) //xem phần sau phần tiêu đề còn dữ liệu nào nữa không
        fwrite(pos + 4 , 1 , len - header_length -4,f ); //ghi vào file phần dữ liệu đó

    //ghi cho tới khi hết nội dung file
    while (1){
        int ret = recv(client, buf, sizeof(buf), 0);
        if(ret <=0){
            break;
        }
       
        fwrite(buf, 1, ret,f );
      
    }


    free(header);
    fclose(f);
    close(client);
    printf("\nDone");
    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <netdb.h>

// int main() 
// {
//     // Download file http://storage.googleapis.com/it4060_samples/ecard.mp4

//     int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

//     // Phan giai ten mien
//     struct addrinfo *info;
//     if (getaddrinfo("storage.googleapis.com", "http", NULL, &info)) {
//         puts("getaddrinfo() failed.");
//         return 1;
//     }

//     int ret = connect(client, info->ai_addr, info->ai_addrlen);
//     if (ret == -1) {
//         perror("connect() failed");
//         return 1;
//     }

//     char req[] = "GET /it4060_samples/ecard.mp4 HTTP/1.1\r\nHost: storage.googleapis.com\r\nConnection: close\r\n\r\n";
//     send(client, req, strlen(req), 0);

//     char buf[2048];
//     // Nhan noi dung phan header
//     char *header = NULL;
//     int len = 0;

//     while (1) {
//         int ret = recv(client, buf, sizeof(buf), 0);
//         if (ret <= 0) {
//             close(client);
//             printf("disconnected\n");
//             return 1;
//         }

//         header = realloc(header, len + ret);
//         memcpy(header + len, buf, ret);
//         len += ret;

//         if (memmem(header, len, "\r\n\r\n", 4) != NULL) {
//             break;
//         }
//     }

//     // In header
//     puts(header);

//     char *pos = memmem(header, len, "\r\n\r\n", 4);

//     FILE *f = fopen("download.mp4", "wb");

//     // Nhan noi dung file
//     int header_length = pos - header;
//     if (header_length + 4 < len)
//         fwrite(pos + 4, 1, len - header_length - 4, f);
    
//     while (1) {
//         int ret = recv(client, buf, sizeof(buf), 0);
//         if (ret <= 0)
//             break;
//         fwrite(buf, 1, ret, f);
//     }

//     free(header);
//     fclose(f);
//     close(client);

//     printf("Done.");
// }