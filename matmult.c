#include<stdlib.h>
#include<pthread.h>

#define Item(X, I, J, NCOL)      X[((J) + (I) * (NCOL))]

typedef struct
{
    int* A;
    int* B;
    int* C;
    int l;
    int m;
    int n;
    int row;
    int column;
}matrix;

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult(int* A, int* B, int* C, int l, int m, int n)
{
    for(int i=0; i<l; i++)
        for(int j=0; j<n; j++)
        {
            int sum = 0;
            for(int k=0; k<m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
            Item(C, i, j, n) = sum;
        }
}
void *matmultelement(void *arg)
{
    matrix mult=*(matrix*)arg;
    int sum = 0;
    for(int k=0; k<mult.m; k++)
        sum += Item(mult.A, mult.row, k, mult.m) * Item(mult.B, k, mult.column, mult.n);
    Item(mult.C, mult.row, mult.column, mult.n) = sum;
}
void *matmultrow(void *arg)
{
    matrix mult=*(matrix*)arg;
    for(int j=0; j<mult.n; j++)
    {
        int sum = 0;
        for(int k=0; k<mult.m; k++)
            sum += Item(mult.A, mult.row, k, mult.m) * Item(mult.B, k, j, mult.n);
        Item(mult.C, mult.row, j, mult.n) = sum;
    }
}



/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult_v1(int* A, int* B, int* C, int l, int m, int n)
{
    pthread_t thread_id[l][n];
    matrix *array[l*n];
    int i, j;
    for(i=0; i < l; i++)
    {
        for(j=0; j<n; j++)
        {
            Item(C, i, j, n) = 0;
        }
    }

    for(i=0; i < l; i++)
    {
        for(j=0; j<n; j++)
        {
            matrix* array=malloc(sizeof(matrix)*l*n);
            array[i].A=A;
            array[i].B=B;
            array[i].C=C;
            array[i].l=l;
            array[i].m=m;
            array[i].n=n;
            array[i].row=i;
            array[i].column=j;
            pthread_create( &thread_id[i][j], NULL, matmultelement, &array[i]);
        }
    }

    for(i=0; i < l; i++)
    {
        for(j=0; j<n; j++)
        {
            pthread_join( thread_id[i][j], NULL); 
        }
    }
}



/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult_v2(int* A, int* B, int* C, int l, int m, int n)
{
    pthread_t thread_id[l];
    matrix *array[l];
    int i;

    for(i=0; i < l; i++)
    {
        for(int j=0; j<n; j++)
        {
            Item(C, i, j, n) = 0;
        }
    }

    for(i=0; i < l; i++)
    {
        matrix* array=malloc(sizeof(matrix)*l);
        array[i].A=A;
        array[i].B=B;
        array[i].C=C;
        array[i].l=l;
        array[i].m=m;
        array[i].n=n;
        array[i].row=i;
        pthread_create( &thread_id[i], NULL, matmultrow, &array[i]);
    }

    for(i=0; i < l; i++)
    {
        
        pthread_join( thread_id[i], NULL); 
        
    }
}