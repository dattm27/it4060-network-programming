/*
Sử dụng hàm select()/poll(), viết chương trình
chat_server thực hiện các chức năng sau:
Nhận kết nối từ các client, và vào hỏi tên client cho đến
khi client gửi đúng cú pháp: “client_id: client_name”
trong đó client_name là tên của client, xâu ký tự viết liền.
Sau đó nhận dữ liệu từ một client và gửi dữ liệu đó đến các client còn lại,
 ví dụ: client có id “abc” gửi “xin chao” thì các client khác sẽ nhận được: “abc: xin chao” hoặc có thể thêm thời gian vào trước ví dụ: “2023/05/06 11:00:00PM abc: xin chao”.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

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

    // khai báo tập fdread chứa các các socket và fdtest để thăm dò sự kiện
    fd_set fdread, fdtest;
    FD_ZERO(&fdread);

    // gan socket listener vao fdread
    FD_SET(listener, &fdread);
    char buf[256];
    // mang luu tru cac client da dang nhap
    int client_socket[FD_SETSIZE];
    int num_clients = 0;
    char *client_names[FD_SETSIZE]; // moi ten 50 ky tu

    while (1)
    {
        // khoi tao lai fd_set = fdread;
        fdtest = fdread; // giữ nguyên các socket ở trong fdread
        int ret = select(FD_SETSIZE, &fdtest, NULL, NULL, NULL);
        if (ret == -1)
        {
            break;
        }
        // kiem tra xem su kien cua socket nao
        //-> kiem tra tat ca cac su kien co the co
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &fdtest))
            { // socket i xay ra su kien
                if (i == listener)
                {
                    // co ket noi -> su kien cua server
                    int client = accept(listener, NULL, NULL);
                    if (client >= FD_SETSIZE)
                    {
                        close(client);
                    }
                    else
                    {
                        // them vao fread
                        FD_SET(client, &fdread);
                        printf("New client connected: %d\n", client);
                    }
                }
                else
                {
                    // co du lieu truyen -> Nhan du liey
                    int client = i;
                    ret = recv(client, buf, sizeof(buf), 0);
                    if (ret <= 0)
                    {
                        FD_CLR(client, &fdread);
                        close(client);

                        // xoa client ra khoi mang dang nhap
                        // xoa id (ten) va xoa socket
                        remove_client(client, client_socket, client_names, &num_clients);
                    }
                    else
                    {
                        buf[ret] = 0;
                        printf("Received from %d: %s\n", client, buf);
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
                                    }

                                    char *msg = "Dung cu phap:\n";
                                    send(client, msg, strlen(msg), 0);
                                    // chuyen client sang trang thai dang nhap
                                    client_socket[num_clients] = client;
                                    // cap phat bo nho
                                    client_names[num_clients] = malloc((strlen(id) + 1));
                                    strcpy(client_names[num_clients], id);
                                    num_clients++;
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
                            if (strcmp(receiver, "all:") == 0)
                            {
                                // chuyen tiep cho cac clients khac da dang nhap
                                for (int k = 0; k < num_clients; k++)
                                {
                                    if (client_socket[k] != client)
                                    {
                                        char msg[512];
                                        sprintf(msg, "%s: %s\n ", client_names[j], buf);
                                        send(client_socket[k], msg, strlen(msg), 0);
                                    }
                                }
                            }
                            else
                            {
                                // chuyen tiep tin nhan cho mot client
                                for (int k = 0; k < num_clients; k++)
                                {

                                    if (strcmp(client_names[k], receiver) == 0)
                                    {
                                        char msg[512];
                                        sprintf(msg, "%s: %s\n ", client_names[j], buf + strlen(receiver) + 1);
                                        send(client_socket[k], msg, strlen(msg), 0);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}

/*
Khong toi uu bang phien ban 1 vi phai kiem tra tat ca
*/