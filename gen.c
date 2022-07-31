#include<stdlib.h>
#include<stdio.h>
#include<time.h>

int main(){
    FILE *f = fopen("sort.input", "w");
    long long s = 100000;
    srand(time(NULL));
    fprintf(f, "%lld\n", 1);
    fprintf(f, "%lld\n", s);
    for(long long i=0;i<s;i++)
        fprintf(f, "%lld ", rand() % 1000);
    fclose(f);
    return 0;
}
