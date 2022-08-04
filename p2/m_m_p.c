#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<stdint.h>

int **m1, **m2, **mm;
int n, cores;

int **create_matrix(int n){
#ifdef DEBUG
    printf("create_matrix\n");
#endif
    int **m;
    m = (int **) malloc(n*sizeof(int*));
    for(int i=0;i<n;i++){
        m[i] = (int*) malloc(n*sizeof(int));
        for(int j=0;j<n;j++)
            m[i][j] = rand() % 5;
    }
    return m;
}

void destroy_matrix(int ***m, int n){
#ifdef DEBUG
    printf("destroy_matrix\n");
#endif
    for(int i=0;i<n;i++)
        free((*m)[i]);
    free(*m);
}

void show(int **m, int n){
#ifdef DEBUG
    printf("show\n");
#endif
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            printf("[%d]", m[i][j]);
        printf("\n");
    }
    printf("\n");
}

void *multiply(void *args)
{
    int64_t id = (int64_t) args;
    int secao = n / cores;
    int start = id * secao;
    int end = (id+1) * secao;

    for(int i=start;i<end;i++)
        for(int j=0;j<n;j++){
            mm[i][j] = 0;
            for(int k=0;k<n;k++)
                mm[i][j] += m1[i][k] * m2[k][j];
        }
}

void main(int argc, char **argv){
    if(!(argc == 3)){
        exit(1);
    }
    n = atoi(argv[1]);
    cores = atoi(argv[2]);
    pthread_t *threads;
    
    srand(time(NULL));

    m1 = create_matrix(n);
    m2 = create_matrix(n);
    mm = create_matrix(n);

    //show(m1, n);
    //show(m2, n);

    threads = (pthread_t *) malloc(cores*sizeof(pthread_t));

    for(int64_t i=0;i<cores;i++)
        pthread_create(threads+i, NULL, multiply, (void *)i);

    for(int64_t i=0;i<cores;i++)
        pthread_join(threads[i], NULL);

    //show(mm, n);
    
    destroy_matrix(&m1, n);
    destroy_matrix(&m2, n);
    destroy_matrix(&mm, n);
    exit(0);
}
