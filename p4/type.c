#include<stdio.h>
#include<inttypes.h>

int main(){
    int8_t a = 11;
    int8_t b = 12;
    printf("%d\n", sizeof((int16_t) a << 8) | b);
    return 0;
}
