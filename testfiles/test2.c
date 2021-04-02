#include <stdio.h>
#include "thread.h"
#include <stdlib.h>


typedef struct matrix{
    int x;
    int y;
}matrix;

typedef struct par_matrix{
    int **A;
    int **B;
    int **C;
    matrix r;
    matrix Q;
}par_matrix;

int **input(int n, int m){
    int **matrix1;
    int i, j;
    matrix1 = (int**)malloc(sizeof(int*) * n);
    for(i = 0; i < n; i++)
        matrix1[i] = (int *)malloc(m * sizeof(int)); 
    for(i = 0; i < n; i++)
        for(j = 0; j < m; j++)
            scanf("%d", &matrix1[i][j]);

    return matrix1;
}
void freeMatrix(int **a, int rows) {
    for(int i = 0; i < rows; i++) {
        free(a[i]);
    }
    free(a);
}

void* mult(void* arr){
    par_matrix *Array = (par_matrix*)arr;
    int i, j, k;
    for(i = Array->r.x; i <  Array->r.y; i++){
        for(j = 0; j < Array->Q.y; j++){
            Array->C[i][j] = 0;
            for(k = 0; k < Array->Q.x; k++){
                Array->C[i][j] += Array->A[i][k] * Array->B[k][j];
            }
           
        }
       
    }
}

int main(){
    matrix A, B;
    int i, j;
    scanf("%d %d", &A.x , &A.y);
    int **matrix1 = input(A.x, A.y);
    scanf("%d %d", &B.x, &B.y);
    int **matrix2 = input(B.x, B.y);
    thread_tcb tid[3], tid1;
    matrix row_arr[3];
    int rows;
    par_matrix arr[3], Matrix;
    int **result;
    if(A.y != B.x){
        printf("Multiplication is not possible\n");
    }else{
        result = (int**)malloc(sizeof(int*) * A.x);
        for(i = 0; i <  A.x; i++)
            result[i] = (int *)malloc(B.y * sizeof(int)); 
        
        if(A.x < 3){
           
            Matrix.A = matrix1;
            Matrix.B = matrix2;
            Matrix.C = result;
            Matrix.r.x = 0;
            Matrix.r.y = A.x;
            Matrix.Q.x = B.x;
            Matrix.Q.y = B.y;
            thread_create(&tid1, mult, &Matrix);
            thread_join(tid1, NULL);
        }else{
            rows = A.x/3;
            row_arr[0].x = 0;
            row_arr[0].y = rows;
            row_arr[1].x = rows;
            row_arr[1].y = 2*rows;
            row_arr[2].x = 2*rows;
            row_arr[2].y = A.x;
            
            for(i = 0; i < 3; i++){
                arr[i].A = matrix1;
                arr[i].B = matrix2;
                arr[i].C = result;
                arr[i].r.x = row_arr[i].x;
                arr[i].r.y = row_arr[i].y;
                arr[i].Q.x = B.x;
                arr[i].Q.y = B.y;
                thread_create(&(tid[i]),mult, &arr[i]); 
            }

            for( i=0; i<3; i++){	
                thread_join(tid[i], NULL);
        }

        }
        
        printf("\n%d %d\n", A.x, B.y);
        for(i = 0; i < A.x; i++){
            for(j = 0; j < B.y; j++)
                printf("%d\t", result[i][j]);
            printf("\n");
        }

        freeMatrix(matrix1, A.x);
        freeMatrix(matrix2, B.x);
        freeMatrix(result, A.x);
    }
    
    return 0;
}
