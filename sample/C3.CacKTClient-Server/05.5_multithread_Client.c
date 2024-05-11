
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
// định nghĩa hàm thực thi của luồng
void *thread_proc(void *);

int main()
{

    // khai bao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // khai bao dia chi cua server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    // ket noi den sever
    int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1)
    {
        printf("Khong ket noi duoc den server");
        return 1;
    }

    pthread_t tid;
    //luồng con chờ dữ nhập dữ liệu từ bàn phím
    pthread_create(&tid, NULL, thread_proc, (void *) &client);
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

void *thread_proc(void *arg)
{

    int client = *(int *)arg;
    char buf[256];
    while (1)
    {
        fgets(buf, sizeof(buf), stdin);
        send(client, buf, strlen(buf), 0);
    }
    close(client);
}