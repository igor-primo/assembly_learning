#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<stdint.h>

double **m1, **m2, **mm;
int cores;
int lins1, cols1;
int lins2, cols2;

double **create_matrix(int lins, int cols){
    printf("create\n");
    double **m = (double **) malloc(lins*sizeof(double*));
    for(int i=0;i<lins;i++){
        m[i] = (double *) malloc(cols*sizeof(double));
        for(int j=0;j<cols;j++)
            m[i][j] = 0.0;
    }
    printf("end create\n");
    return m;
}

void destroy_matrix(double ***m, int lins){
    for(int i=0;i<lins;i++)
        free((*m)[i]);
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
    printf("id = %ld, secao = %d, start = %d, end = %d\n", id, secao, start, end);
    /*
    for(int i=start;i<end;i++)
        for(int j=0;j<cols2;j++)
            for(int k=0;k<cols1;k=k+11){
                mm[i][j] += m1[i][k] * m2[k][j];
                mm[i][j] += m1[i][k+1] * m2[k+1][j];
                mm[i][j] += m1[i][k+2] * m2[k+2][j];
                mm[i][j] += m1[i][k+3] * m2[k+3][j];
                mm[i][j] += m1[i][k+4] * m2[k+4][j];
                mm[i][j] += m1[i][k+5] * m2[k+5][j];
                mm[i][j] += m1[i][k+6] * m2[k+6][j];
                mm[i][j] += m1[i][k+7] * m2[k+7][j];
                mm[i][j] += m1[i][k+8] * m2[k+8][j];
                mm[i][j] += m1[i][k+9] * m2[k+9][j];
                mm[i][j] += m1[i][k+10] * m2[k+10][j];
            }
    */
    /*
    if(cols1 % 2 == 0)
        for(int i=start;i<end;i++)
            for(int j=0;j<cols2;j++)
                for(int k=0;k<cols1;k=k+2){
                    mm[i][j] += m1[i][k] * m2[k][j];
                    mm[i][j] += m1[i][k+1] * m2[k+1][j];
                }
    else
        for(int i=start;i<end;i++)
            for(int j=0;j<cols2;j++)
                for(int k=0;k<cols1-1;k=k+2){
                    mm[i][j] += m1[i][k] * m2[k][j];
                    mm[i][j] += m1[i][k+1] * m2[k+1][j];
                }
    */
    /*
    for(int i=start;i<end;i++)
        for(int j=0;j<cols2;j++)
            for(int k=0;k<cols1;k++)
                mm[i][j] += m1[i][k] * m2[k][j];
    */
    for(int i=start;i<end;i++){
        double * linha_m1 = m1[i];
        double * linha_mm = mm[i];
        for(int j=0;j<cols2;j++){
            double * linha_m2 = m2[j];
            double ika = linha_m1[j];
            for(int k=0;k<cols1;k++){
                linha_mm[k] += ika * linha_m2[k];
            }
        }
    }
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
        for(int i=0;i<lins1;i++)
            for(int j=0;j<cols1;j++)
                fscanf(in, "%lf", &m1[i][j]);
        fscanf(in, "%d %d", &lins2, &cols2);
        m2 = create_matrix(lins2, cols2);
        for(int i=0;i<lins2;i++)
            for(int j=0;j<cols2;j++)
                fscanf(in, "%lf", &m2[i][j]);
        mm = create_matrix(lins1, cols2);
        cores = 4;
        if(lins1 < cores)
            cores = lins1;
        threads = (pthread_t *) malloc(cores*sizeof(pthread_t));
        for(int64_t i=0;i<cores;i++)
            pthread_create(&threads[i], NULL, &multiply, (void *)i);
        for(int64_t i=0;i<cores;i++)
            pthread_join(threads[i], NULL);
        fprintf(out, "M%d:\n", trick - (tests + 1));
        for(int i=0;i<lins1;i++)
            for(int j=0;j<cols2;j++){
                fprintf(out, " ");
                fprintf(out, "%0.2lf", mm[i][j]);
                if(j == lins1-1)
                    fprintf(out, "\n");
            }
        destroy_matrix(&m1, lins1);
        destroy_matrix(&m2, lins2);
        destroy_matrix(&mm, lins1);
        free(threads);
    }
    fclose(in);
    fclose(out);
    return 0;
}
