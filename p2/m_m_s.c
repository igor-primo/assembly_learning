#include<stdio.h>
#include<stdlib.h>
#include<time.h>

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
}

void multiply(m1, m2, mm, n)
    int **m1;
    int **m2;
    int **mm;
    int n;
{
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
            mm[i][j] = 0;
            for(int k=0;k<n;k++)
                mm[i][j] += m1[i][k] + m2[k][j];
        }
}

void main(int argc, char **argv){
    if(!(argc == 2)){
        exit(1);
    }
    srand(time(NULL));
    int n = atoi(argv[1]);

    int **m1 = create_matrix(n);
    int **m2 = create_matrix(n);
    int **mm = create_matrix(n);

    show(m1, n);
    show(m2, n);

    multiply(m1, m2, mm, n);
    show(mm, n);
    
    destroy_matrix(&m1, n);
    destroy_matrix(&m2, n);
    destroy_matrix(&mm, n);
    exit(0);
}
