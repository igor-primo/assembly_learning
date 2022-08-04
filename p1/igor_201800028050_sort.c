#include<stdio.h>
#include<inttypes.h>
#include<stdlib.h>

void swap(uint16_t *arr, uint16_t i, uint16_t min){
    uint16_t aux = arr[i];
    arr[i] = arr[min];
    arr[min] = aux;
}

void sort(uint16_t *arr, uint16_t size){
    // sel sort
    for(uint16_t i; i < size-1; i++){
        uint16_t min = i;
        for(uint16_t j=i+1;j < size; j++)
            if(arr[j] < arr[min])
                min = j;
        if(min != i)
            swap(arr, i, min);
    }
}

int main(int argc, char *argv[]){
    if(!(argc == 3))
        exit(1);
    uint16_t tests_num = 0;
    char *in = argv[1];
    char *out = argv[2];
    FILE *f_in = fopen(in, "r");
    FILE *f_out = fopen(out, "w");
    if(f_in == NULL || f_out == NULL)
        exit(1);
    fscanf(f_in, "%"SCNu16"", &tests_num);
    uint16_t trick = tests_num;
    while(tests_num--){
        uint16_t size = 0;
        fscanf(f_in, "%"SCNu16"", &size);
        uint16_t *arr = (uint16_t *) malloc(size*sizeof(uint16_t));
        uint16_t s = size;
        uint16_t i = 0;
        while(size--) fscanf(f_in, "%"SCNu16"", arr+(i++));
        sort(arr, s);
        fprintf(f_out, "[%"SCNu16"] ", trick - (tests_num + 1));
        i = 0;
        while(s--) fprintf(f_out, "%"SCNu16" ", arr[i++]);
        fprintf(f_out, "\n");
        free(arr);
    }
    fclose(f_in);
    fclose(f_out);
    return 0;
}
