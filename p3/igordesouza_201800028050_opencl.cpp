#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<stdint.h>

double *m1, *m2, *mm;
int cores;
int lins1, cols1;
int lins2, cols2;

double *create_matrix(int lins, int cols){
    double *m = (double *) malloc(lins*cols*sizeof(double));
    int mult = lins*cols;
    for(int i=0;i<mult;i++)
        m[i] = 0.0;
    return m;
}

void destroy_matrix(double **m){
    free(*m);
}

void show(double **m, int lins, int cols){
    for(int i=0;i<lins;i++){
        for(int j=0;j<cols;j++)
            printf("[%0.2lf]", m[i][j]);
        printf("\n");
    }
    printf("\n");
}

void *multiply(void *args){
    long id = (long) args;
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
    //printf("id = %ld, secao = %d, start = %d, end = %d\n", id, secao, start, end);
    /*
    for(int i=start;i<end;i++)
        for(int j=0;j<cols2;j++)
            for(int k=0;k<cols1;k++)
                mm[i][j] += m1[i][k] * m2[k][j];
    */
    /*
    for(int i=start;i<=end;i++)
        for(int j=0;j<cols2;j++)
            for(int k=0;k<cols1;k++)
                mm[i*cols2+j] += m1[i*cols1+k] * m2[k*cols2+j];
    */
    int i = start;
    do {
        for(int j=0;j<cols2;j++)
            for(int k=0;k<cols1;k++)
                mm[i*cols2+j] += m1[i*cols1+k] * m2[k*cols2+j];
        i++;
    } while(i<end);
    return NULL;
}

int main(int argc, char **argv){
    if(!(argc == 3))
        return 1;
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");
    if(in == NULL || out == NULL)
        return 1;
    pthread_t *threads;
    int tests = 0;

    fscanf(in, "%d", &tests);
    int trick = tests;
    while(tests--){
        fscanf(in, "%d %d", &lins1, &cols1);
        m1 = create_matrix(lins1, cols1);
        int m1_ = lins1 * cols1;
        for(int i=0;i<m1_;i++)
                fscanf(in, "%lf", &m1[i]);
        fscanf(in, "%d %d", &lins2, &cols2);
        m2 = create_matrix(lins2, cols2);
        int m2_ = lins2 * cols2;
        for(int i=0;i<m2_;i++)
                fscanf(in, "%lf", &m2[i]);
        mm = create_matrix(cols2, lins1);
        cores = lins1;
        if(lins1 < cores)
            cores = lins1;
        //threads = (pthread_t *) malloc(cores*sizeof(pthread_t));
        for(int64_t i=0;i<cores;i++);
            //pthread_create(&threads[i], NULL, &multiply, (void *)i);
        for(int64_t i=0;i<cores;i++);
            //pthread_join(threads[i], NULL);
        fprintf(out, "M%d:\n", trick - (tests + 1));
        for(int i=0;i<cols2;i++){
            for(int j=0;j<lins1;j++)
                fprintf(out, " %0.2lf", mm[i*cols2+j]);
            fprintf(out, "\n");
        }
        destroy_matrix(&m1);
        destroy_matrix(&m2);
        destroy_matrix(&mm);
        free(threads);
    }
    fclose(in);
    fclose(out);
    return 0;
}
