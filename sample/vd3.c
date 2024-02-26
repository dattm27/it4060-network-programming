/*
 VD3: Chuỗi ký tự sau là phản hồi của lệnh PASV trong giao thức FTP, 
 hãy xác định giá trị địa chỉ IP và cổng.
 227 Entering Passive Mode (213,229,112,130,216,4)
*/

#include<stdio.h>
#include<string.h>

int main() {
    char resp[]= "227 Entering Passive Mode (213,229,112,130,216,4)";
    //tìm vị trí dấu mở ngoặc 
    char *pos = strchr(resp, '(');
    char *p1 = strtok(pos, "(),");
    char *p2 = strtok(NULL, "(),");
    char *p3 = strtok(NULL, "(),");
    char *p4 = strtok(NULL, "(),");

    printf("IP: %s.%s.%s.%s", p1, p2, p3, p4);
    
    return 0;
}