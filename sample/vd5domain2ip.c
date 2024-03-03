// Phân giải tên miền 
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>

int main () {
    struct addrinfo *res, *p; 
    int ret = getaddrinfo("vnexpress.net", "http", NULL, &res);
    if (ret != 0) {
        printf("Khong phan giai duoc");
        return 1;
    }
    //Duyet danh sach ket qua
    p = res;
    while (p != NULL){
        if (p->ai_family == AF_INET){
            printf("IPv4\n");
            struct sockaddr_in addr;
            memcpy(&addr, p-> ai_addr,  sizeof(addr));
            printf("IP: %s\n", inet_ntoa(addr.sin_addr));
        }
        else if (p->ai_family == AF_INET6){
            printf("IPv6\n");
        }
        p = p->ai_next;
    }
    return 0;
}