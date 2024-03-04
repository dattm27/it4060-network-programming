#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<unistd.h>
#include<arpa/inet.h>
int main () {
    //tao socket cho ket noi 
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //khai bao cau truc dia chi
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    //gan socket voi cau truc dia chi 
   if(bind(listener, (struct sockaddr *) &addr, sizeof(addr))){
        perror("bind() failed.\n");
        return 1;
   };

   //chuyen socket sang listen
   if (listen(listener, 5)){
    perror("listen() failed.\n");
    return 1;
   }
    printf("Dang cho ket noi...\n");

    //in ra dc IP cong client dang ket noi 
    struct sockaddr_in clientAddr;
    unsigned int clientAddrLen = sizeof(clientAddr);

    //chap nhan ket noi 
    int client = accept(listener, (struct sockaddr *) &clientAddr, &clientAddrLen);
    printf("Client moi ket noi: %d \n", client);
    printf("IP: %s\n", inet_ntoa(clientAddr.sin_addr));
    printf("Port: %d\n", ntohs(clientAddr.sin_port));

/*    //gui xau
    char msg[] = "Hello client";
    int ret = send(client, msg, sizeof(msg), 0); // xem gui di duoc bnh byte
    printf("%d bytes da duoc gui di\n", ret);

    //gui chuoi 
    char buf[256];
    for (int i = 0; i<10 ; i++){
        buf[i] = i +97;
    }
    ret = send(client, buf, 10, 0);
    printf("%d bytes da duoc gui di\n", ret);

*/

/*    //nhan du lieu 
    printf("Dang nhan du lieu...\n");
    char buf[16];
    memset(buf, 97, sizeof(buf));
    int ret = recv(client, buf, sizeof(buf), 0);
    if(ret ==0){
        printf("Ket noi bi huy\n");
        return 1;

    }
    printf("So bytes nhan dipc %d\n", ret);
    //them ky tu ket thuc xau
    buf[ret]=0; // chi in du lieu nhan duoc
    printf("Du lieu nhan duoc: %s\n", buf);
*/
    //nhan du lieu nhieu lan
    while (1){
        printf("Dang nhan du lieu...\n");
        char buf[16];
        memset(buf, 97, sizeof(buf));
        int ret = recv(client, buf, sizeof(buf), 0);
        if(ret ==0){
            printf("Ket noi bi huy\n");
            return 1;
        }
        printf("So bytes nhan dipc %d\n", ret);
        //them ky tu ket thuc xau
        buf[ret]=0; // chi in du lieu nhan duoc
        printf("Du lieu nhan duoc: %s\n", buf);

    }

    return 0; 

}
