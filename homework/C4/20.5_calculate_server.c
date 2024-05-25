/*
Sinh viên lập trình tạo trang web thực hiện các phép tính đơn giản:
+ Nhận tham số từ lệnh GET
+ Nhận tham số từ lệnh POST
Các tham số bao gồm:
+ a, b: 2 tham số là 2 toán hạng kiểu số thực
+ cmd: tham số phép tính cần thực hiện, gồm các giá trị add, sub, mul, div ứng với các phép tính cộng, trừ, nhân, chia.
Server trả lại kết quả là trang web hiển thị phép tính và kết quả.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

// Hàm để gửi phản hồi HTTP
void send_response(int client, const char *message) {
    char response[4096];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "%s", message);
    send(client, response, strlen(response), 0);
}

// Hàm để tách và tìm giá trị tham số từ chuỗi query
char* get_param_value(const char* query, const char* param) {
    char *p = strstr(query, param);
   
    if (p) {
        p += strlen(param) + 1; // Bỏ qua tên tham số và dấu '='
        char *end = strchr(p, '&');
        if (end) {
            *end = '\0';
        }
        
        return p;
    }
    return NULL;
}

// Hàm xử lý yêu cầu từ client
void handle_client(int client) {
    char buffer[4096];
    int bytes_received = recv(client, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        perror("recv() failed");
        close(client);
        return;
    }

    buffer[bytes_received] = '\0';
    printf("Received request:\n%s\n", buffer);

    // Tìm phần query trong yêu cầu GET
    char *query ;
    char *get_query = strstr(buffer, "GET /");
    char *post_query =  strstr(buffer, "POST /");
    if (get_query || post_query) {
        if (get_query){
            query = get_query ;
            query += strlen("GET /");
        }
        else {
            query = post_query ;
            query += strlen("POST /");
        }
        char *end = strchr(query, ' ');
        if (end) {
            *end = '\0';
        }

        // Tách và lấy giá trị của các tham số
        char *y_str = get_param_value(query, "y");
       char *x_str = get_param_value(query, "x");
        char *cmd= get_param_value(query, "cmd");
      
        printf(" request: %s %s %s\n", cmd, x_str , y_str);
        if (cmd && x_str && y_str) {
            int x = atoi(x_str);
            int y = atoi(y_str);
            int result;

            if (strcmp(cmd, "add") == 0) {
                result = x + y;
                cmd = "+";
            } else if (strcmp(cmd, "subtract") == 0) {
                result = x - y;
                  cmd = "-";
            } else if (strcmp(cmd, "multiply") == 0) {
                result = x * y;
                  cmd = "*";
            } else if (strcmp(cmd, "divide") == 0) {
                if (y == 0) {
                    send_response(client, "<html><body><h1>Error: Division by zero</h1></body></html>");
                    close(client);
                    return;
                }
                result = x / y;
                cmd = "/";
            } else {
                send_response(client, "<html><body><h1>Error: Invalid operation</h1></body></html>");
                close(client);
                return;
            }

            // Trả lại kết quả phép tính
            char response_message[256];
            snprintf(response_message, sizeof(response_message),
                "<html><body><h1>%d %s %d = %d</h1></body></html>",x , cmd, y   , result);
            send_response(client, response_message);
        } else {
            send_response(client, "<html><body><h1>Error: Missing parameters</h1></body></html>");
        }
    } else {
        send_response(client, "<html><body><h1>Error: Invalid request</h1></body></html>");
    }

    close(client);
}

int main() {
    // Tạo socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1) {
        perror("socket() failed");
        return 1;
    }

    // Khai báo địa chỉ server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    // Gắn socket với cấu trúc địa chỉ
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind() failed");
        return 1;
    }

    // Chuyển socket sang trạng thái chờ kết nối
    if (listen(listener, 5)) {
        perror("listen() failed");
        return 1;
    }

    printf("Server is listening on port 9000...\n");

    while (1) {
        // Chấp nhận kết nối từ client
        int client = accept(listener, NULL, NULL);
        if (client == -1) {
            perror("accept() failed");
            continue;
        }

        printf("New client connected: %d\n", client);

        if (fork() == 0) {
            // Tiến trình con, xử lý yêu cầu từ client
            close(listener);
            handle_client(client);
            exit(0);
        }

        // Đóng socket client ở tiến trình cha
        close(client);
    }

    return 0;
}
