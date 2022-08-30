#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<stdint.h>

double **m1, **m2, **mm;
int cores;
int lins1, cols1;
int lins2, cols2;
FILE *in, *out;

double **create_matrix(int lins, int cols){
    fprintf(out, "create_matrix\n");
    double **m = (double **) malloc(lins*sizeof(double*));
    for(int i=0;i<lins;i++){
        m[i] = (double *) malloc(cols*sizeof(double));
        for(int j=0;j<cols;j++)
            m[i][j] = 0.0;
    }
    return m;
}

void destroy_matrix(double ***m, int lins){
    fprintf(out, "destroy_matrix\n");
    for(int i=0;i<lins;i++)
        free((*m)[i]);
    free(*m);
}

void show(double **m, int lins, int cols){
    fprintf(out, "show\n");
    for(int i=0;i<lins;i++){
        for(int j=0;j<cols;j++)
            fprintf(out, "[%0.2lf]", m[i][j]);
        fprintf(out, "\n");
    }
    fprintf(out, "\n");
}

void *multiply(void *args)
{
    long id = (long) args;
    //int64_t id = (int64_t) args;
    int secao = 0, start = 0, end = 0;
    if(cores == 5 && id != 4){
        secao = lins1 / (cores-1);
        start = id * secao;
        end = start + secao;
    } else if(cores == 5){
        secao = lins1 % (cores-1);
        start = lins1 - secao;
        end = lins1;
    } else {
        secao = lins1 / cores;
        start = id * secao;
        end = start + secao;
    }
    fprintf(out, "id = %ld, secao = %d, start = %d, end = %d\n", id, secao, start, end);
    for(int i=start;i<end;i++)
        for(int j=0;j<cols2;j++)
            for(int k=0;k<cols1;k++)
                mm[i][j] += m1[i][k] * m2[k][j];
    return NULL;
}

int main(int argc, char **argv){
    if(!(argc == 3))
        return 1;
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    fprintf(out, "%d\n", in == NULL);
    fprintf(out, "%d\n", out == NULL);
    if(in == NULL || out == NULL)
        return 1;
    pthread_t *threads;
    int tests = 0;

    fscanf(in, "%d", &tests);
    fprintf(out, "%d\n", tests);

    int trick = tests;
    while(tests--){
        fscanf(in, "%d %d", &lins1, &cols1);
        fprintf(out, "%d %d\n", lins1, cols1);
        m1 = create_matrix(lins1, cols1);
        for(int i=0;i<lins1;i++)
            for(int j=0;j<cols1;j++)
                fscanf(in, "%lf", &m1[i][j]);
        fscanf(in, "%d %d", &lins2, &cols2);
        fprintf(out, "%d %d\n", lins2, cols2);
        m2 = create_matrix(lins2, cols2);
        for(int i=0;i<lins2;i++)
            for(int j=0;j<cols2;j++)
                fscanf(in, "%lf", &m2[i][j]);
        //show(m1, lins1, cols1);
        //show(m2, lins2, cols2);
        mm = create_matrix(cols2, lins1);

        cores = 4;
        cores = lins1 <= cores ? lins1 : 
            (lins1 % cores == 0 ? cores : cores+1);
        fprintf(out, "cores == %d\n", cores);
        threads = (pthread_t *) malloc(cores*sizeof(pthread_t));
        for(int64_t i=0;i<cores;i++)
            pthread_create(&threads[i], NULL, &multiply, (void *)i);
        for(int64_t i=0;i<cores;i++)
            pthread_join(threads[i], NULL);
        //show(mm, cols2, lins1);
        fprintf(out, "M%d:\n", trick - (tests + 1));
        for(int i=0;i<cols2;i++)
            for(int j=0;j<lins1;j++){
                fprintf(out, " ");
                fprintf(out, "%0.2lf", mm[i][j]);
                if(j == lins1-1)
                    fprintf(out, "\n");
            }

        destroy_matrix(&m1, lins1);
        destroy_matrix(&m2, lins2);
        destroy_matrix(&mm, cols2);
        free(threads);
    }
    fclose(in);
    fclose(out);
    return 0;
}
