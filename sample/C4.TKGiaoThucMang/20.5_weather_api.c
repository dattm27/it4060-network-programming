/*
OK
Đọc kết quả của yêu cầu GET tới http://api.weatherapi.com/v1/current.json?key=48bab0abac324847925230945232306&q=Hanoi&aqi=no
và cho biết nhiệt độ (C);
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

    // phan giai ten mien
    struct addrinfo *info;
    int ret = getaddrinfo("api.weatherapi.com", "http", NULL, &info);
    if (ret != 0)
    {
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

    char req[] = "GET /v1/current.json?key=48bab0abac324847925230945232306&q=Hanoi&aqi=no HTTP/1.1\r\nHost:api.weatherapi.com\r\n\r\n";
    // char req2[] = "POST /post HTTP/1.1\r\nHost:httpbin.org\r\nContent-Length: 27\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nparam1=value1&param2=value2";
    send(client, req, strlen(req), 0);
    char buf[2048];
    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            break;
        }
        buf[ret] = 0;
        char *pos = strstr(buf, "\"temp_c\":");
        char *pos1 = strchr(pos, ':') + 1;
        char *pos2 = strchr(pos, ',');
       
        char temp_buf[32] = {0 };
        memcpy(temp_buf, pos1, pos2-pos1 );
        float temp = atoi(temp_buf);
    
        printf("Giá trị nhiệt độ (C): %f\n", temp);
    }
    close(client);
    return 0;
}
