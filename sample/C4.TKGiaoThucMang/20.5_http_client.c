/*
1. Lập trình ứng dụng HTTP client
a. Gửi lệnh GET đến trang http://httpbin.org/get
b. Gửi lệnh POST đến trang http://httpbin.org/post
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

    //phan giai ten mien http://httpbin.org
    struct addrinfo *info;
    int ret = getaddrinfo("httpbin.org", "http", NULL, &info);
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

    char req[] = "GET /get?param1=value1&param2=value2 HTTP/1.1\r\nHost:httpbin.org\r\n\r\n";
    char req2[] = "POST /post HTTP/1.1\r\nHost:httpbin.org\r\nContent-Length: 27\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nparam1=value1&param2=value2";
    send(client, req2, strlen(req2), 0);
    char buf[256];
    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            break;
        }
        buf[ret] = 0;
        puts(buf);
    }
    close(client);
    return 0;
}
