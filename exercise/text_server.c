#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define BUFFER_SIZE 15
int count_pattern(const char * data, const char *cache){
    const char *pattern = "0123456789";
    int count = 0;
    char temp[strlen(data) + strlen(cache)]; // Tạo một bản sao của chuỗi data để xử lý
    //copy cache vào temp
    strcpy(temp, cache);

    //nối data vào temp
    strcat(temp, data);
   
    char *found = temp;
    while ((found = strstr(found, pattern)) != NULL) {
        count++;
        found += strlen(pattern); // Di chuyển con trỏ đến vị trí sau vị trí tìm thấy pattern
    }
   
    return count;


}
int main () {
    int count = 0 ; 
    //tao socket cho ket noi 
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener ==-1){
        perror("socket() failed.\n");
        exit (1);
    }

    //khai bao cau truc dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    //gan socket voi cau truc dia chi
    if (bind(listener, (struct sockaddr_in *) &addr, sizeof(addr))){
        perror("bind() failed\n");
        exit (1);
    }

    //chuyen socket sang trang thai cho ket noi
    if (listen(listener,5)){
        perror("listen () failed\n");
        exit (1);
    }
    else printf("Waiting for connection ...\n");

    //chap nhan ket noi cua client
    int client = accept(listener, NULL, NULL);
    printf("Client connected: %d \n", client);

    //nhan noi dung cua client gui den
    char buf[BUFFER_SIZE];
    char cache[10];
    while (1) {
        int ret = recv(client, buf, sizeof(buf), 0 );
        if (ret <= 0) {
            break;
        }
        if (ret < BUFFER_SIZE) buf[ret] = '\0';
        //đếm số lượng "0123456789" trong cache + xâu nhận được 
        count += count_pattern(buf, cache); 
        //giữ lại 9 ký tự cuối cùng của xâu
        memcpy(cache, buf + strlen(buf) -9, 9 );

    }
    printf("Co %d xau \"0123456789\" trong xau nhan duoc.\n", count);
   
    return 0;
}