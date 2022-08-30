#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>
#include<pthread.h>
#include<sys/mman.h>
#include<assert.h>
#include<string.h>
#include<errno.h>

typedef uint64_t i64;

typedef struct {
    i64 index;
    i64 id;
} ARGS;

typedef struct {
    double *a;
    double *b;
    double *c;
    i64 lins1;
    i64 cols1;
    i64 lins2;
    i64 cols2;
} TASK;

TASK *tasks;

i64 cores;

FILE *in;
FILE *out;

void *write(void *arg){
    i64 index = (i64) arg;
    double **c = tasks[index].c;
    i64 lins1 = tasks[index].lins1;
    i64 cols2 = tasks[index].cols2;
    fprintf(out, "M%"SCNu64":\n", index);
    for(i64 i=0;i<lins1;i++){
        for(i64 j=0;j<cols2;j++)
            fprintf(out, " %0.2lf", c[i][j]);
        fprintf(out, "\n");
    }
}

void *multiply_rl(void *arg){
    ARGS *args = (ARGS*) arg;
    i64 index = args->index;
    i64 id = args->id;

    i64 lins1 = tasks[index].lins1;
    i64 cols2 = tasks[index].cols2;
    i64 cols1 = tasks[index].cols1;
    double *a = tasks[index].a;
    double *b = tasks[index].b;
    double *c = tasks[index].c;

    int secao = 0, start = 0, end = 0;
    if(lins1 == cores){
        start = id;
        end = start + secao;
    } else if(lins1 % cores == 0){
        secao = lins1 / cores;
        start = id * secao;
        end = start + secao;
    } else if(id != 3){
        secao = lins1 / cores;
        start = id * secao;
        end = start + secao;
    } else {
        secao = (lins1 / cores) + (lins1 % cores);
        start = lins1 - secao - 1;
        end = lins1;
    }
    printf("id = %ld, secao = %d, start = %d, end = %d\n", id, secao, start, end);

    /*
    for(int i=start;i<end+1;i++)
        for(int j=0;j<cols2;j++)
            for(int k=0;k<cols1;k++)
                c[i][j] += a[i][k] * b[k][j];
    */
    return NULL;
}

void *multiply(void *arg){
    printf("inside multiply\n");
    i64 index = (i64) arg;
    // multiply matrices in tasks[index] by using other threads
    cores = tasks[index].lins1;
    pthread_t *multiply_rl_th = (pthread_t*) malloc(cores*sizeof(pthread_t));
    ARGS *args = malloc(sizeof(ARGS));
    args->index = index;
    for(i64 i=0;i<cores;i++){
        args->id = i;
        pthread_create(&multiply_rl_th[i], NULL, &multiply_rl, (void*)args);
    }
    /*
    args->id = 0;
    pthread_create(&multiply_rl_th[0], NULL, &multiply_rl, (void*)args);
    args->id = 1;
    pthread_create(&multiply_rl_th[1], NULL, &multiply_rl, (void*)args);
    args->id = 2;
    pthread_create(&multiply_rl_th[2], NULL, &multiply_rl, (void*)args);
    i64 lins1 = tasks[index].lins1;
    i64 cols2 = tasks[index].cols2;
    i64 cols1 = tasks[index].cols1;
    double **a = tasks[index].a;
    double **b = tasks[index].b;
    double **c = tasks[index].c;
    for(int i=0;i<lins1;i++)
        for(int j=0;j<cols2;j++)
            for(int k=0;k<cols1;k++)
                c[i][j] += a[i][k] * b[k][j];
    */
    free(multiply_rl_th);
    return NULL;
}

void *read(void *id){
    printf("read\n");
    i64 tests = (i64) id;
    tasks = (TASK *) malloc(tests*sizeof(TASK));
    i64 trick = tests;
    while(tests--){
        i64 index = trick - tests - 1;
       fscanf(in, "%"SCNu64"""%"SCNu64"", &(tasks[index].lins1), \
               &(tasks[index].cols1));
       printf("creating matrix 1\n");
       // create matrix 1
       i64 lins, cols;
       lins = tasks[index].lins1;
       cols = tasks[index].cols1;
       tasks[index].a = mmap(NULL, lins*cols*sizeof(double), PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
       if(tasks[index].a == MAP_FAILED){
           printf("error: %s\n", strerror(errno));
           return NULL;
       }
       i64 siz = lins * cols;
       for(i64 i=0;i<siz;i++)
           fscanf(in, "%lf", &(tasks[index].a[i]));
       fscanf(in, "%"SCNu64"""%"SCNu64"", &(tasks[index].lins2), \
               &(tasks[index].cols2));
       printf("creating matrix 2\n");
       // create matrix 2
       lins = tasks[index].lins2;
       cols = tasks[index].cols2;
       tasks[index].b = (double *) mmap(0, lins*cols*sizeof(double), PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
       siz = lins * cols;
       for(i64 i=0;i<siz;i++)
                fscanf(in, "%lf", &tasks[index].b[i]);
       printf("creating matrix 3\n");
        // create matrix result
       tasks[index].c = (double *) mmap(0, lins*cols*sizeof(double), PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
       lins = tasks[index].lins1;
       cols = tasks[index].cols2;
       siz = lins * cols;
       for(i64 i=0;i<siz;i++)
                tasks[index].c[i] = 0.0;
       printf("multiplication\n");
        // create thread to multiply
        pthread_t multiply_th;
        //pthread_create(&multiply_th, NULL, &multiply, (void*)index);
    }
    return NULL;
}

int main(int argc, char **argv){
    if(!(argc == 3))
        return 1;
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if(in == NULL || out == NULL)
        return 1;
    i64 tests = 0; // problem
    fscanf(in, "%"SCNu64"", &tests);
    pthread_t read_th;
    pthread_create(&read_th, NULL, &read, (void*) tests);
    pthread_join(read_th, NULL);
    /*
    for(i64 i=0;i<tests;i++)
        write((void*)i);
        */
    fclose(in);
    fclose(out);
    return 0;
}
