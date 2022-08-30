#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char **argv){
    if(!(argc == 4)){
        printf("usage: %s %s %s %s", argv[0], "num_tests", "max_array_size", "max_int");
        return 1;
    }
    srand(time(NULL));
    int tests = atoi(argv[1]);
    int max_array_size = atoi(argv[2]);
    int max_int = atoi(argv[3]);
    FILE *f = fopen("sort.input", "w");
    if(f == NULL)
        return 1;
    fprintf(f, "%d\n", tests);
    while(tests--){
        int size = rand() % max_array_size + 100;
        int *arr = (int*) malloc(size*sizeof(int));
        fprintf(f, "%d\n", size);
        for(int i=0;i<size;i++)
            fprintf(f, "%d ", rand() % max_int);
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}
