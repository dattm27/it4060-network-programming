#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>
const int BUFFER_SIZE = 256;
void signalHandler(int signo)
{
    int stat;
    pid_t pid = wait(&stat); // id của tiến trình con vừa kết thúc
    printf("Child %d terminated.\n", pid);
}

int main()
{

    // tạo socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    // Khai bao dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    // Gan socket voi cau truc dia chi
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind() failed");
        return 1;
    }

    // Chuyen socket sang trang thai cho ket noi
    if (listen(listener, 5))
    {
        perror("listen() failed");
        return 1;
    }

    signal(SIGCHLD, signalHandler);

    while (1)
    {
        printf("Waiting for client...\n");
        int client = accept(listener, NULL, NULL);
        bool logined = false;
        if (fork() == 0)
        {
            // dong socket listener
            close(listener);
            char buf[256];
            while (1)
            {
                int ret = recv(client, buf, sizeof(buf), 0);
                if (ret <= 0)
                    break;
                buf[ret] = 0;
                puts(buf);

                if (logined)
                {
                    // chạy lệnh và gửi kết qua r
                    //da dang nhap
                            char cmd[64];

                            //thuc hien ;emj 
                            if(buf[strlen(buf)-1] == '\n' ){
                                buf[strlen(buf)-1] = 0;
                            }
                          
                            sprintf(cmd, "%s > out.txt", buf);
                            system(cmd );

                            //tra loi ket qua cjp client
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
                else
                {
                    // chưa đăng nhập
                    char user[32], pass[32], tmp[65], line[65];
                    int n = sscanf(buf, "%s %s %s", user, pass, tmp);
                    if (n != 2)
                    {
                        char *msg = "Sai cú pháp. Hãy nhập lại\n";
                        send(client, msg, strlen(msg), 0);
                    }
                    else
                    {
                        // kiem tra thonfg tin dang nhap
                        sprintf(tmp, "%s %s\n", user, pass);

                        FILE *f = fopen("account.txt", "rb");
                        int found = 0;
                        while (fgets(line, sizeof(line), f) != NULL)
                        {

                            if (strcmp(tmp, line) == 0)
                            {
                                found = 1;
                                break;
                            }
                        }
                        if (found)
                        {
                            char *msg = "Dang nhap thanh cong.\n";
                            logined = true;
                            send(client, msg, strlen(msg), 0);
                        }
                        else
                        {
                            char *msg = "TK ko ton tai. Thu lai.\n";
                            send(client, msg, strlen(msg), 0);
                        }
                        fclose(f);
                    }
                }

            }
            exit(0);
        }
        // dong socket client o tien trinh cha
        close(client);
    }
}
