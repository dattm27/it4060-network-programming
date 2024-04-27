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
#define BUFFER_SIZE 256
// Cấu trúc lưu trữ thông tin tài khoản
typedef struct {
    char username[50];
    char password[50];
} Account;

// Hàm đọc dữ liệu từ file cơ sở dữ liệu vào mảng các tài khoản
int read_database(Account *database, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening database file");
        return -1;
    }

    int count = 0;
    while (fscanf(file, "%s %s", database[count].username, database[count].password) != EOF) {
        count++;
    }

    fclose(file);
    return count;
}

int main() {

    // Khởi tạo mảng lưu trữ thông tin tài khoản
    Account database[MAX_FDS];
    int num_accounts = read_database(database, "account.txt");
    if (num_accounts == -1) {
        return 1;
    }

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
    int client_log[MAX_FDS];
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

                        //yeu cau client nhap mat khau
                        char *msg = "Enter username and password: ";
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
                        client_log[client] = 0; //dang xuat tai client   

                    } else {
                        // Xu ly du lieu nhan duoc
                        
                        printf("Received from %d: %s\n", client, buf);
                        
                        //so sanh tai khoan va mat khau trong co so du lieu
                        // đọc tài khoản + mk client gửi theo định dạng
                        if (client_log[client] !=1){
                            char username[50], password[50];
                            if (sscanf(buf, "%s %s", username, password) != 2) {
                                // Sai định dạng, thông báo lỗi và đóng kết nối
                                char *error_msg = "Invalid format. Try again: ";
                                send(client, error_msg, strlen(error_msg), 0);
                               
                        
                            }
                            //kiểm tra xem có giống tk nào trong database k 
                            else {
                                // So sánh tên người dùng và mật khẩu với dữ liệu trong cơ sở dữ liệu
                                int authenticated = 0;
                                for (int j = 0; j < num_accounts; j++) {
                                    if (strcmp(username, database[j].username) == 0 &&
                                        strcmp(password, database[j].password) == 0) {
                                        authenticated = 1;
                                        break;
                                    }
                                }
                                if ( !authenticated) {
                                    char *error_msg = "Incorrect username or password. Try again: ";
                                    send(client, error_msg, strlen(error_msg), 0);
                                }
                                else client_log[client] = 1;
                                
                            }
                        }
                        else {
                            
                            char cmd[BUFFER_SIZE] = "";
                            buf[ret-1] = '\0';
                            strcpy(cmd, buf);
                            strcat(cmd, " > out.txt");
                            //thực hiện lệnh
                            system(cmd);

                            //gửi cho client đó kết quả
                             //doc du lieu tu file de gui di
                            FILE *f = fopen("out.txt", "rb");
                            if (f == NULL){
                                printf("Cannot open file.\n");
                                exit(1);
                            }
                            char buf[BUFFER_SIZE];

                            //truyen noi dung file den client
                            fseek(f,0 , SEEK_SET);
                            while (1) {
                                int ret = fread(buf, 1 ,sizeof(buf), f);
                                if (ret <=0) break;
                                //neu trong lan doc cuoi cung so ky tu con lai < BUFFER_SIZE
                                if(ret < BUFFER_SIZE) buf[ret] = '\0';

                               
                                send(client, buf, ret,0);
                            }
                            printf("Sent successfully!");
                            fclose(f);

                            
                        }
                        
                    }
                }
            }
        }
    }
    
    return 0;
}
