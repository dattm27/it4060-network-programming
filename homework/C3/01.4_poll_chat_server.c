#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>

#define MAX_FDS 2048
#define MAX_MSG 2048

int main() {
    // Tao socket cho ket noi
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1) {
        perror("socket() failed");
        return 1;
    }

    // Khai bao dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);

    // Gan socket voi cau truc dia chi
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind() failed");
        return 1;
    }

    // Chuyen socket sang trang thai cho ket noi
    if (listen(listener, 5)) {
        perror("listen() failed");
        return 1;
    }

    struct pollfd fds[MAX_FDS];
    int nfds = 0;

    // Gan socket listener vao tap tham do
    fds[0].fd = listener;
    fds[0].events = POLLIN;
    nfds++;
    char client_id[MAX_FDS][50];
    char buf[256];

    while (1) {
        int ret = poll(fds, nfds, 0);
        if (ret == -1) {
            break;
        }
        if (ret == 0) {
            // printf("Timed out!!!\n");
            continue;
        }

        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == listener) {
                    // Co ket noi moi
                    int client = accept(listener, NULL, NULL);
                    if (client >= MAX_FDS) {
                        // Vuot qua so luong toi da
                        close(client);
                    } else {
                        // Them vao mang tham do su kien
                        fds[nfds].fd = client;
                        fds[nfds].events = POLLIN;
                        nfds++;

                        printf("New client connected: %d\n", client);

                        //hoi ten client
                        char *msg = "Enter your name: (Syntax: client_id: client_name): ";
                        send(client, msg, strlen(msg),0);
                    }
                } else {
                    // Co du lieu tu client truyen den
                    int client = fds[i].fd;
                   
                    ret = recv(client, buf, sizeof(buf), 0);
                    buf[ret] = 0;
                    if (ret <= 0) {
                        // Ket noi bi dong => Xoa khoi mang
                        fds[i] = fds[nfds-1];
                        nfds--;
                        i--;
                        strcpy(client_id[client] ,"");

                    } else {
                        // Xu ly du lieu nhan duoc
                        
                        printf("Received from %d: %s\n", client, buf);
                        //khi chưa có tên của client trong mảng
                        if (strcmp(client_id[client], "")==0){
                             // gửi lại yêu cầu tiên cho tới khi đúng cú pháp
                            if (strstr(buf, "client_id: ")== NULL){
                                char *msg = "Enter your name: (Syntax: client_id: client_name): ";
                                send(client, msg, strlen(msg),0);
                            }
                            else {
                            // thêm tên của client vào mảng
                            buf[ret-1]=0;
                            strcpy(client_id[client], buf+10);  
                            
                            }
                        }
                        //gửi cho các client khác
                        else{
                            // tạo ngày tháng
                            time_t rawtime;
                            struct tm *info;
                            char datetime[80];
                            time( &rawtime );
                            info = localtime( &rawtime );
                             strftime(datetime, 80, "%Y/%m/%d %I:%M:%S%p", info); // định dạng 
                            
                            //nối vào thông điệp gửi đi 
                            char message[MAX_MSG] = "";
                            strcat(message, datetime);
                            strcat(message, client_id[client]);
                            strcat(message, ": ");
                            strcat(message,  buf);
                            for(int j = 1 ; j < nfds ; j++ ){
                                int other = fds[j].fd ;
                                if (other != client) send(other , message, strlen(message), 0);
                            }
                        }
                       
                    }
                }
            }
        }
    }

    return 0;
}
