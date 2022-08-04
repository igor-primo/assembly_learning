#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<stdint.h>

double **m1, **m2, **mm;
int cores;
int lins1, cols1;
int lins2, cols2;

void destroy_matrix(double ***m, int lins){
#ifdef DEBUG
    printf("destroy_matrix\n");
#endif
    for(int i=0;i<lins;i++)
        free((*m)[i]);
    free(*m);
}

void show(double **m, int lins, int cols){
#ifdef DEBUG
    printf("show\n");
#endif
    for(int i=0;i<lins;i++){
        for(int j=0;j<cols;j++)
            printf("[%lf]", m[i][j]);
        printf("\n");
    }
    printf("\n");
}

void *multiply(void *args)
{
    int64_t id = (int64_t) args;
    int secao = lins1 / cores;
    int start = id * secao;
    int end = (id+1) * secao;

    for(int i=start;i<end;i++)
        for(int j=start;j<end;j++){
            mm[i][j] = 0.0;
            for(int k=start;k<end;k++)
                mm[i][j] += m1[i][k] * m2[k][j];
        }
}

int main(int argc, char **argv){
    FILE *in = fopen("pthread_in.txt", "r");
    FILE *out = fopen("pthread_out.txt", "w");
#ifdef DEBUG
    printf("%d\n", in == NULL);
    printf("%d\n", out == NULL);
#endif
    if(in == NULL || out == NULL)
        exit(1);
#ifdef DEBUG
    printf("read\n");
#endif
    cores = 4;
    pthread_t *threads;
    int tests = 0;

    fscanf(in, "%d", &tests);
#ifdef DEBUG
    printf("%d\n", tests);
#endif

    while(tests--){
        fscanf(in, "%d %d", &lins1, &cols1);
#ifdef DEBUG
        printf("%d %d\n", lins1, cols1);
#endif
        for(int i=0;i<lins1;i++){
            printf("%d\n", i);
            for(int j=0;j<cols1;j++){
                printf("%d", j);
                fscanf(in, "%lf", &m1[i][j]);
            }
        }

        fscanf(in, "%d %d", &lins2, &cols2);
#ifdef DEBUG
        printf("%d %d\n", lins2, cols2);
#endif
        for(int i=0;i<lins2;i++)
            for(int j=0;j<cols2;j++)
                fscanf(in, "%lf", &m2[i][j]);
#ifdef DEBUG

        show(m1, lins1, cols1);
        show(m2, lins1, cols2);
#endif
        threads = (pthread_t *) malloc(cores*sizeof(pthread_t));

        for(int64_t i=0;i<cores;i++)
            pthread_create(threads+i, NULL, multiply, (void *)i);

        for(int64_t i=0;i<cores;i++)
            pthread_join(threads[i], NULL);
#ifdef DEBUG
        show(mm, lins1, cols2);
#endif
        destroy_matrix(&m1, lins1);
        destroy_matrix(&m2, lins2);
        destroy_matrix(&mm, lins1);
        free(threads);
    }
    fclose(in);
    fclose(out);
    return 0;
}
