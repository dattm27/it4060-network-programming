// Ví dụ tham số dòng lệnh
#include<stdio.h>
#include<stdlib.h>
// argc là số tham số đã truyền
// argv là mảng các tham số kiểu chuỗi
// tham số đầu tiên là tên file thực thi
int main (int agrc, char *agrv[]) {

    int n= atoi(agrv[1]);
    for (int i = 1 ; i<= n ; i++) printf("%d ", i);
    return 0;
}