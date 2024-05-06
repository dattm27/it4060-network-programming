/*
Bài 2: Lập trình ứng dụng time_server thực hiện chức năng sau:
+ Chấp nhận nhiều kết nối từ các client.
+ Cient gửi lệnh GET_TIME [format] để nhận thời gian từ server.
+ format là định dạng thời gian server cần trả về client. Các format cần hỗ trợ gồm:
    dd/mm/yyyy – vd: 30/01/2023
    dd/mm/yy – vd: 30/01/23
    mm/dd/yyyy – vd: 01/30/2023
    mm/dd/yy – vd: 01/30/23
+ Cần kiểm tra tính đúng đắn của lệnh client gửi lên server.
Bài gốc: time_sever giao ngày 22.04
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
#include <sys/select.h>
#define CLIENTS_SL 1024
// mang luu tru cac client da dang nhap
int client_socket[CLIENTS_SL];
int num_clients = 0;
char *client_names[CLIENTS_SL]; // moi ten 50 ky tu

// xử lý luồng client
void *client_proc(void *arg);

// xoá một client ngắt kết nối
int remove_client(int client, int *client_sockets, char **client_names, int *num_clients)
{
    // xoa socket , xoa ten, num_clients giam di 1
    int i = 0;
    for (; i < num_clients; i++)
    {
        if (client_sockets[i] == client)
            break;
    }
    if (i < *num_clients)
    {
        if (i < *num_clients - 1)
        {
            // gan thong tin phan tu cuoi = phan tu tai i
            client_sockets[i] = client_sockets[*num_clients - 1];
            strcpy(client_names[i], client_names[*num_clients - 1]);
        }

        // giai phong bo nho
        free(client_names[*num_clients - 1]);
        *num_clients -= 1;
    }
}

int main()
{
    // khởi tạo server
    // tao socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener != -1)
        printf("Socket created: %d\n", listener);
    else
    {
        printf("Failed to create socket.\n");
        exit(1);
    }

    // Khai bao cau truc dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    // Gan dia chi voi socket
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
    {
        printf("bind() failed.\n ");
        exit(1);
    }

    if (listen(listener, 5))
    {
        printf("listen() failed.\n");
        exit(1);
    }
    else
        printf("Waiting for a new client...\n");

    char buf[256];

    pthread_t tid;

    while (1)
    {

        // mỗi khi có client kết nối tới

        // co ket noi -> su kien cua server
        int client = accept(listener, NULL, NULL);
        if (client >= CLIENTS_SL)
        {
            close(client);
        }
        else
        {

            printf("New client connected: %d\n", client);
            pthread_t tid;
            pthread_create(&tid, NULL, client_proc, (void *)&client);
            pthread_detach(tid);
        }
    }
    return 0;
}

void *client_proc(void *arg)
{
    char buf[256];
    int client = *(int *)arg;
    while (1)
    {
        // Chờ dữ liệu từ client
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            continue;

        // xử lý dữ liệu, gửi về cho client
        buf[ret] = 0;
        printf("Received from client %d: %s\n", client, buf);
        // Kiểm tra cú pháp của lệnh mà client gửi GET_TIME [format
        char cmd[32], format[32], tmp[32];
        int n = sscanf(buf, "%s %s %s", cmd, format, tmp);
        if (n == 2 && strcmp(cmd, "GET_TIME") == 0)
        {
            time_t rawtime;
            struct tm *timeinfo;
            char date[32];

            // Lấy thời gian hiện tại
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            if (strcmp(format, "dd/mm/yyyy") == 0)
            {
                strftime(date, sizeof(date), "%d/%m/%Y", timeinfo);
            }
            else if (strcmp(format, "dd/mm/yy") == 0)
            {
                strftime(date, sizeof(date), "%d/%m/%y", timeinfo);
            }
            else if (strcmp(format, "mm/dd/yyyy") == 0)
            {
                strftime(date, sizeof(date), "%m/%d/%Y", timeinfo);
            }
            else if (strcmp(format, "mm/dd/yy") == 0)
            {
                strftime(date, sizeof(date), "%m/%d/%y", timeinfo);
            }
            else
            {
                strcpy(date, "Sai cu phap. GET_TIME [format]");
            }
            strcat(date, "\n");
            send(client, date, strlen(date), 0);
        }
        else
        {
            // trả lại dữ liệu cho client
            char *msg = "Sai cu phap. GET_TIME [format]";
            send(client, msg, strlen(msg), 0);
        }
    }
}