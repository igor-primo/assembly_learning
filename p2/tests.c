#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char **argv){
    if(!(argc == 4)){
        printf("usage: %s %s %s %s\n", \
                argv[0], "tests", "max_matrix_size", "max_value");
        return 1;
    }
    srand(time(NULL));
    int tests = atoi(argv[1]);
    int max_matrix_size = atoi(argv[2]);
    int max_value = atoi(argv[3]);
    FILE *f = fopen("pthread.input", "w");
    if(f == NULL)
        return 1;
    fprintf(f, "%d\n", tests);
    while(tests--){
        int lins1 = rand() % max_matrix_size;
        int cols1 = rand() % max_matrix_size;
        int lins2 = cols1;
        int cols2 = lins1;
        fprintf(f, "%d %d\n", lins1, cols1);
        for(int i=0;i<lins1;i++){
            for(int j=0;j<cols1;j++)
                fprintf(f, " %0.2lf", (double) (rand() % max_value));
            fprintf(f, "\n");
        }
        fprintf(f, "%d %d\n", lins2, cols2);
        for(int i=0;i<lins2;i++){
            for(int j=0;j<cols2;j++)
                fprintf(f, " %0.2lf", (double) (rand() % max_value));
            fprintf(f, "\n");
        }
    }
    return 0;
}
