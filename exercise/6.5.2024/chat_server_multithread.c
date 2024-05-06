
/*
Sử dụng hàm select()/poll(), viết chương trình
chat_server thực hiện các chức năng sau:
Nhận kết nối từ các client, và vào hỏi tên client cho đến
khi client gửi đúng cú pháp: “client_id: client_name”
trong đó client_name là tên của client, xâu ký tự viết liền.
Sau đó nhận dữ liệu từ một client và gửi dữ liệu đó đến các client còn lại,
ví dụ: client có id “abc” gửi “xin chao” thì các client khác sẽ nhận được:
“abc: xin chao” hoặc có thể thêm thời gian vào trước ví dụ: “2023/05/06 11:00:00PM abc: xin chao”.
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

void *client_proc(void *arg);

int remove_client(int client, int *client_sockets, char **client_names, int *num_clients){
    //xoa socket , xoa ten, num_clients giam di 1
    int i = 0;
    for (; i < num_clients ; i++){
        if(client_sockets[i]==client) break;
    }
    if(i < *num_clients){
        if(i < *num_clients-1) {
            //gan thong tin phan tu cuoi = phan tu tai i
            client_sockets[i] = client_sockets[*num_clients -1];
            strcpy(client_names[i],client_names[*num_clients -1]);
            

        }

        //giai phong bo nho
        free(client_names[*num_clients-1]);
        *num_clients-= 1;

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
    // co du lieu truyen -> Nhan du liey
    int client = *(int *)arg;
    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {

            //client đã ngắt kết nối
            remove_client(client, client_socket, client_names, &num_clients);
            break;
        }
        else
        {
            buf[ret] = 0;
            printf("Received from %d: %s\n", client, buf);
        }
        // kiem tra trang thai dang nhap
        int j;
        for (j = 0; j < num_clients; j++)
        {
            if (client_socket[j] == client)
                break;
        }
        if (j == num_clients)
        {
            // chua dang nhap
            char cmd[16], id[32], tmp[32];
            // su dung sscanf kiem tra dinh dang du lieu nhap vao
            int n = sscanf(buf, "%s %s %s", cmd, id, tmp);
            if (n == 2)
            {
                if (strcmp(cmd, "client_id:") == 0)
                {
                    // kiem tra d ton tai chua
                    int k;
                    for (k = 0; k < num_clients; k++)
                    {
                        // kiem tra
                        if (strcmp(client_names[k], id) == 0)
                            break; //
                    }
                    if (k < num_clients)
                    {
                        // id da ton tai,, gui thong bao loi
                        char *msg = "ID da ton tai. Thu lai\n";
                        send(client, msg, strlen(msg), 0);
                    }
                    else
                    {
                        char *msg = "Dang nhap thanh cong\n";
                        send(client, msg, strlen(msg), 0);
                        // chuyen client sang trang thai dang nhap
                        client_socket[num_clients] = client;
                        // cap phat bo nho
                        client_names[num_clients] = malloc((strlen(id) - 1));

                        strcpy(client_names[num_clients], id);

                        num_clients++;
                    }
                }
                else
                {
                    // sai cu phap
                    char *msg = "Sai cu phap 1. Hay nhap lai:\n";
                    send(client, msg, strlen(msg), 0);
                }
            }
            else
            {
                // sai cu phap
                char *msg = "Sai cu phap 2. Hay nhap lai:\n";
                send(client, msg, strlen(msg), 0);
            }
        }
        else
        {

            // da dang nhap
            // kiem tra
            char receiver[32];
            // phan tich tu buf nhan duoc
            sscanf(buf, "%s", receiver);
            // printf("receiver: %s\n", receiver);
            if (strcmp(receiver, "all:") == 0)
            {
                // chuyen tiep cho cac clients khac da dang nhap
                for (int k = 0; k < num_clients; k++)
                {
                    if (client_socket[k] != client)
                    {
                        char msg[512];
                        sprintf(msg, "%s: %s\n", client_names[j], buf +strlen(receiver) + 1);
                        send(client_socket[k], msg, strlen(msg), 0);
                    }
                }
            }
            else
            {
                // printf("Private msg\n");
                receiver[strlen(receiver) - 1] = 0; // bỏ dấu hai chấm đi
                // chuyen tiep tin nhan cho mot client
                for (int k = 0; k < num_clients; k++)
                {
                    // printf("client_name: %s, receiver: %s\n", client_names[k], buf );
                    if (strcmp(client_names[k], receiver) == 0)
                    {
                        // printf("Match\n");
                        char msg[512];
                        sprintf(msg, "%s: %s\n", client_names[j], buf + strlen(receiver) + 1);
                        send(client_socket[k], msg, strlen(msg), 0);
                    }
                }
            }
        }
    }
    close(client);
}

