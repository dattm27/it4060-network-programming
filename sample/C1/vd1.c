//VD1: Cho mảng chứa nội dung là header của gói tin IP 
//hãy in ra giá trị version, ihl, total_length, 
//địa chỉ IP nguồn và đích.
#include<stdio.h>
unsigned char header[] = {   0x45, 0x00, 0x00, 0x40,                         
                    0x7c, 0xda, 0x40, 0x00,                         
                    0x80, 0x06, 0xfa, 0xd8,                        
                    0xc0, 0xa8, 0x0f, 0x0b,                         
                    0xbc, 0xac, 0xf6, 0xa4};
int main () {
    int version  = (header[0] >> 4) & 0xFF;
    printf("Version: %d\n", version);
    int ihl = (header[0] &0xF)*4; 
    printf("IHL: %d bytes\n", ihl);
    int totalLength = header[2] * 256 + header[3]; 
    printf("Tototal length: %d bytes\n", totalLength);
    printf("Source ID: %d.%d.%d.%d\n", 
        header[12], header[13], header[14], header[15]);
    printf("Source ID: %d.%d.%d.%d\n", 
        header[16], header[17], header[17], header[18]);
    return 0;
}