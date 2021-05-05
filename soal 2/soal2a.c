#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MATRIX_A_ROW 4
#define MATRIX_A_COL 3
#define MATRIX_B_ROW 3
#define MATRIX_B_COL 6

int matrix_a[MATRIX_A_ROW][MATRIX_A_COL],
    matrix_b[MATRIX_B_ROW][MATRIX_B_COL],
    (*matrix_result)[MATRIX_B_COL];
// matrix_result[MATRIX_A_ROW][MATRIX_B_COL];

void getMatrixInput()
{
    //retrieve matrix a
    printf("Matrix A:\n");
    int row, col;
    for (row = 0; row < MATRIX_A_ROW; row++)
    {
        for (col = 0; col < MATRIX_A_COL; col++)
        {
            scanf("%d", &matrix_a[row][col]);
        }
    }

    //retrieve matrix b

    printf("Matrix B:\n");
    for (row = 0; row < MATRIX_B_ROW; row++)
    {
        for (col = 0; col < MATRIX_B_COL; col++)
        {
            scanf("%d", &matrix_b[row][col]);
        }
    }
}

void performMatrixMultiplication()
{
    int row, col;
    for (row = 0; row < MATRIX_A_ROW; row++)
    {
        for (col = 0; col < MATRIX_B_COL; col++)
        {
            int result = 0,
                index;

            for (index = 0; index < MATRIX_A_COL; index++)
            {
                result += matrix_a[row][index] * matrix_b[index][col];
            }

            matrix_result[row][col] = result;
        }
    }
}

void printResultMatrixUtil()
{
    int row, col;
    for (row = 0; row < MATRIX_A_ROW; row++)
    {
        for (col = 0; col < MATRIX_B_COL; col++)
        {
            printf("%d ", matrix_result[row][col]);
        }
        printf("\n");
    }
}

int main()
{
    key_t key = 1234;
    int shared_mem_id = shmget(key, sizeof(int[MATRIX_A_ROW][MATRIX_B_COL]), IPC_CREAT | 0666);
    matrix_result = shmat(shared_mem_id, NULL, 0);

    if (matrix_result == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }

    //main logi
    getMatrixInput();
    performMatrixMultiplication();
    printResultMatrixUtil();

    shmdt(matrix_result);
    // shmctl(shared_mem_id, IPC_RMID, NULL);
}

/*
1 2 3
1 2 3
1 2 3
1 2 3

1 2 1 2 1 2
1 2 1 2 1 2
1 2 1 2 1 2
*/