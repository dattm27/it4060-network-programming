/*
VD2: Lệnh gửi từ client là chuỗi ký tự có dạng “CMD X Y”
 trong đó CMD là các lệnh ADD, SUB, MUL, DIV, X và Y là 2 số thực. 
 Viết đoạn chương trình kiểm tra một chuỗi ký tự có theo cú pháp trên không, 
 xác định các giátrịcủa CMD, Xvà Y.
*/ 
#include<stdio.h>
#include<string.h>

int main () {
    //char *data = "ADD 3 4 5";
    char data[32];
    char cmd[6], tmp[16];// tmp: biến thừa -> phát hiện sai cú pháp (thừa)
    float x,y;
    
    //nhập lệnh từ bàn phím
    printf("Command: ");
    fgets(data, 32, stdin);
    
    //đọc dữ liệu theo định dạng -> lấy số lượng biến nhập vào 
    int n = sscanf(data, "%s%f%f%s", cmd, &x, &y, tmp );
    //trả về số dữ liệu đọc được thành công
   
    printf("n = %d\n", n);
    printf("cmd = %s\n", cmd);
    printf("x = %f\n", x);
    printf("y = %f", y);
    if (n==3){
        if( strcmp(cmd, "ADD") == 0 || 
            strcmp(cmd, "SUB") == 0 ||
            strcmp(cmd, "MUL") == 0 ||
            strcmp(cmd, "DIV") == 0 )    
        {
            //thực hiện lệnh 
        }
        else {
            printf("Command not supported");
        }
    }
    else {
        //sai cú pháp
        printf("Syntax Error");
    }
    return 0;
}