#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MATRIX_FACT_ROW 4
#define MATRIX_FACT_COL 6
#define MAX_THREAD 100

int matrix_fact[MATRIX_FACT_ROW][MATRIX_FACT_COL],
    (*matrix_multi_result)[MATRIX_FACT_COL];

unsigned long long int matrix_fact_result[MATRIX_FACT_ROW][MATRIX_FACT_COL];

typedef struct permutation_t
{
    int a,
        b,
        row,
        col;

    unsigned long long int *write_to;

} Permutation;

void getMatrixInput()
{
    //retrieve factorial matrix
    printf("Matrix C:\n");
    int row, col;
    for (row = 0; row < MATRIX_FACT_ROW; row++)
    {
        for (col = 0; col < MATRIX_FACT_COL; col++)
        {
            scanf("%d", &matrix_fact[row][col]);
        }
    }
}

void printMultiResultMatrixUtil()
{
    int row, col;
    for (row = 0; row < MATRIX_FACT_ROW; row++)
    {
        for (col = 0; col < MATRIX_FACT_COL; col++)
        {
            printf("%d ", matrix_multi_result[row][col]);
        }
        printf("\n");
    }
}

void printResultMatrixUtil()
{
    int row, col;
    for (row = 0; row < MATRIX_FACT_ROW; row++)
    {
        for (col = 0; col < MATRIX_FACT_COL; col++)
        {
            printf("%d ", matrix_fact_result[row][col]);
        }
        printf("\n");
    }
}

unsigned long int factorial(int num)
{
    int i;
    unsigned long int result = 1;
    for (i = 1; i <= num; i++)
    {
        result = result * i;
    }
    return result;
}

void *calculateCell(void *arg)
{
    Permutation *input = (Permutation *)arg;
    unsigned long int result;
    int a = input->a,
        b = input->b;

    if (a >= b)
    {
        result = factorial(a) / factorial(a - b);
    }
    else if (a < b)
    {
        result = factorial(a);
    }
    else if (a == 0 || b == 0)
    {
        result = 0;
    }

    *(input->write_to) = result;

    return NULL;
}

void performMatrixCellPermutation()
{
    pthread_t thread[MATRIX_FACT_ROW][MATRIX_FACT_COL];
    int row, col;
    for (row = 0; row < MATRIX_FACT_ROW; row++)
    {
        for (col = 0; col < MATRIX_FACT_COL; col++)
        {
            Permutation permutation;
            permutation.a = matrix_multi_result[row][col];
            permutation.b = matrix_fact[row][col];
            permutation.row = row;
            permutation.col = col;
            permutation.write_to = &matrix_fact_result[row][col];

            pthread_create(&thread[row][col], NULL, calculateCell, &permutation);
            sleep(1);
        }
    }

    for (row = 0; row < MATRIX_FACT_ROW; row++)
    {
        for (col = 0; col < MATRIX_FACT_COL; col++)
        {
            pthread_join(thread[row][col], NULL);
        }
    }

    return;
}

int main()
{
    key_t key = 1234;
    int shared_mem_id = shmget(key, sizeof(int[MATRIX_FACT_ROW][MATRIX_FACT_COL]), IPC_CREAT | 0666);
    matrix_multi_result = shmat(shared_mem_id, NULL, 0);

    if (matrix_multi_result == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }

    printMultiResultMatrixUtil();

    //main logic
    getMatrixInput();
    performMatrixCellPermutation();
    printResultMatrixUtil();

    shmdt(matrix_multi_result);
    // shmctl(shared_mem_id, IPC_RMID, NULL);
}

/*
1 2 3  
4 5 6
7 8 9
10 11 12

1 2 3 4 5 6
7 8 9 10 11 12
13 14 15 16 17 18

1 3 2 5 2 1
4 3 2 5 2 1
4 3 2 5 2 1
4 3 2 5 2 1
*/