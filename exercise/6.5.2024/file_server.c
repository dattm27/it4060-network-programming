/*
OK
Sử dụng kỹ thuật đa tiến trình hoặc đa luồng, lập trình ứng dụng file server đơn giản như sau:
Khi client mới được chấp nhận, server sẽ gửi danh sách các file cho client. Các file này trong thư mục được thiết lập trên server. Dòng đầu danh sách là chuỗi “OK N\r\n” nếu có N file trong thư mục. Các tên file phân cách nhau bởi ký tự \r\n, kết thúc danh sách bởi ký tự \r\n\r\n.
Nếu không có file nào trong thư mục thì server gửi chuỗi “ERROR No files to download \r\n” rồi đóng kết nối.
Client gửi tên file để tải về, server kiểm tra nếu file tồn tại thì gửi chuỗi “OK N\r\n” trong đó N là kích thước file, sau đó là nội dung file cho client và đóng kết nối, nếu file không tồn tại thì gửi thông báo lỗi và yêu cầu client gửi lại tên file.

Yêu cầu nộp bài:
+ Mã nguồn chương trình
+ Ảnh chụp màn hình kết quả chạy thử
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
// mang luu tru cac client 
int client_socket[1024];
int num_clients = 0;


int main()
{
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
        if (client >= FD_SETSIZE)
        {
            close(client);
        }
        else
        {

            printf("New client connected: %d\n", client);
            num_clients++;
            pthread_t tid;
            pthread_create(&tid, NULL, client_proc, (void *)&client);
            pthread_detach(tid);
        }
    }
    return 0;
}

void *client_proc(void *arg)
{

    // co du lieu truyen -> Nhan du liey
    int client = *(int *)arg;

    char buf[1024]; // de dong goi du lieu
    char path[256];
    int pos = 0;
    getcwd(path, sizeof(path)); // lay duong dan thu muc hien tai
    puts(path);

    strcpy(buf, path);
    buf[strlen(path)] = '\n';
    pos += strlen(path) + 1;
    // Mở thư mục
    DIR *d = opendir(path);
    struct dirent *dir;
    struct stat st;
    if (d != NULL)
    {
        // Đếm số lượng file trong thư mục
        int file_count = 0;
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {
                file_count++;
            }
        }

        if (file_count > 0)
        {
            // Gửi chuỗi "OK N\r\n" nếu có ít nhất một file trong thư mục
            sprintf(buf, "OK %d\r\n", file_count);

            // Đặt vị trí cho dòng tiếp theo trong buf
            pos = strlen(buf);

            // Reset lại con trỏ vị trí trong thư mục để đọc lại từ đầu
            rewinddir(d);

            // Lặp qua các file trong thư mục và thêm tên file và kích thước vào buf
            while ((dir = readdir(d)) != NULL)
            {
                if (dir->d_type == DT_REG)
                {
                    stat(dir->d_name, &st);

                    // Thêm tên file vào buf
                    strcpy(buf + pos, dir->d_name);
                    strcat(buf + pos, "\r\n");

                    // Tính toán và chuyển đổi kích thước file thành chuỗi
                    char size_str[20];
                    sprintf(size_str, "%ld", st.st_size);

                    // Thêm kích thước vào buf
                    // strcat(buf + pos, size_str);

                    // Cập nhật vị trí cho vòng lặp
                    pos = strlen(buf);
                }
            }
            strcat(buf + pos, "\r\n");
            pos = strlen(buf);
            // Gửi dữ liệu đến client
            send(client, buf, strlen(buf), 0);
        }
        else
        {
            // Gửi chuỗi "ERROR No files to download \r\n" nếu không có file nào trong thư mục
            strcpy(buf, "ERROR No files to download \r\n");
            send(client, buf, strlen(buf), 0);

            // Đóng kết nối với client
            close(client);
        }

        // Đóng thư mục sau khi sử dụng
        closedir(d);
    }
    else
    {
        // Xử lý lỗi khi không mở được thư mục
        perror("opendir");
        // Đóng kết nối với client
        close(client);
    }

    // gui sang server
    // int ret = send(client, buf, pos, 0);

    // printf("%d bytes sent.\n", ret);
    // Nhận tên file từ serverv
    int ret = recv(client, buf, sizeof(buf), 0);
    buf[ret - 1] = '\0';
    printf("Received from %d: %s\n", client, buf);

    // Xử lý gửi file
    //  Kiểm tra xem tệp có tồn tại không
    FILE *file = fopen(buf, "rb");
    if (file != NULL)
    {
        // Gửi thông báo "OK N\r\n"
        fseek(file, 0, SEEK_END);
        int file_size = ftell(file);
        rewind(file);
        sprintf(buf, "OK %d\r\n", file_size);
        send(client, buf, strlen(buf), 0);

        // Gửi nội dung của tệp đến client
        while (1)
        {
            int bytes_read = fread(buf, 1, sizeof(buf), file);
            if (bytes_read <= 0)
                break;
            send(client, buf, bytes_read, 0);
        }

        printf("File sent successfully!\n");

        // Đóng tệp và đóng kết nối với client
        fclose(file);
        close(client);
    }
    else
    {
        // Gửi thông báo lỗi nếu tệp không tồn tại
        strcpy(buf, "ERROR File not found\r\n");
        send(client, buf, strlen(buf), 0);
        close(client);
    }

    // kiem tra trang thai dang nhap
}