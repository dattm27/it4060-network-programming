/*
Sử dụng kỹ thuật đa luồng, lập trình ứng dụng chat nhóm 2 người như sau:
Mỗi client khi kết nối đến server sẽ được lưu vào hàng đợi, nếu số lượng client trong hàng đợi là 2 thì 2 client này sẽ được ghép cặp với nhau.
Khi 2 client đã được ghép cặp thì tin nhắn gửi từ client này sẽ được chuyển tiếp sang client kia và ngược lại.
Nếu 1 trong 2 client ngắt kết nối thì client còn lại cũng được ngắt kết nối.
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
#include <dirent.h>
#include <sys/stat.h>
void *client_proc(void *arg);
#define CLIENTS_SL 1024
// mang luu tru cac client 
int client_socket[CLIENTS_SL];
int num_clients = 0;
int main () {
    // khởi tạo server
    // tao socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener != -1){
        printf("Socket created: %d\n", listener);
    }
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

    while (1)
    {
        // co ket noi -> su kien cua server
        int client = accept(listener, NULL, NULL);
        if (client >= CLIENTS_SL)
        {
            close(client);
        }
        else
        {

            printf("New client connected: %d\n", client);
            num_clients++;
            client_socket[num_clients] = client;
            pthread_t tid;
            pthread_create(&tid, NULL, client_proc, (void *)&client);
            pthread_detach(tid);
        }
    }
    return 0;
}

void *client_proc(void *arg){

    // co du lieu truyen -> Nhan du liey
    int client = *(int *)arg;
    // Đợi cho tới khi có đủ 2 client để ghép cặp
    while (num_clients % 2 != 0) {

        usleep(100000); // Ngủ 100ms để tránh chiếm nhiều tài nguyên CPU
    }

    // Ghép cặp hai client
    int paired_client;
    if (client == client_socket[num_clients]) {
        paired_client = client_socket[num_clients-1];
    } else {
        paired_client = client_socket[num_clients];
    }

    // Chuyển tiếp tin nhắn từ client này sang client kia và ngược lại
    char buf[256];
    int bytes_received;
    while ((bytes_received = recv(client, buf, sizeof(buf), 0)) > 0) {
        buf[bytes_received] = '\0';
        printf("Received message from client %d: %s\n", client, buf);
          // Gửi tin nhắn kèm số thứ tự của client
        char msg[256];
        sprintf(msg, "Client %d: %s", client, buf);
       
        send(paired_client, msg, strlen(msg), 0);
    }
       
    

    // Nếu một trong hai client ngắt kết nối, đóng kết nối với client còn lại
    printf("Client %d disconnected\n", client);
    close(client);
    close(paired_client);

    // Loại bỏ client khỏi mảng
    for (int i = 1; i <= num_clients; i++) {
        if (client_socket[i] == client) {
            for (int j = i; j < num_clients; j++) {
                client_socket[j] = client_socket[j + 1];
            }
            num_clients--;
            break;
        }
    }

    pthread_exit(NULL);
}   