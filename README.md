# soal-shift-sisop-modul-3-B02-2021

## NOMOR 1

## NOMOR 2

### 2A. Membuat sebuah program yang dapat melakukan perkalian matrix 4 x 3 dengan 3 x 6.

#### Penyelesaian

```C++
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
}
```

#### Penjelasan

Fungsi `getMatrixInput()` hanyalah sebuah fungsi utilitas, yang digunakan untuk mengambil input matrix A dan matrix B yang kemudian disimpan dalam sebuah global variable `matrix_a[MATRIX_A_ROW][MATRIX_A_COL]` dan `matrix_b[MATRIX_B_ROW][MATRIX_B_COL]`.

Fungsi `printResultMatrixUtil()` hanyalah sebuah fungsi utilitas, yang digunakan untuk mengoutputkan hasil perkalian matrix.

Fungsi `performMatrixMultiplication()` akan melakukan perkalian matrix yang dibutuhkan, yang hasilnya kemudian dimasukkan ke sebuah global variable yang juga merupakan sebuah shared memory sehingga dapat digunakan di proses yang lain.

ketika `main()` berjalan, maka akan dibentuk sebuah shared memory dengan size `int[MATRIX_A_ROW][MATRIX_B_COL]` yang kemudian di attach ke global variable `matrix_result` yang kemudian variable tersebut digunakan untuk menyimpan hasil dari perkalian matrix. Setelah shared memory terbentuk, maka program akan meminta input, melakukan kalkulasi, dan mengoutputkan hasil dari perkalian.

### 2B. Membuat sebuah program yang mengambil output dari program 2A yang melakukan sebuah perhitungan yang melibatkan matrix baru dengan syarat : 1. If a >= b -> a!/(a-b)! 2. If b > a -> a! dan 3. If 0 -> 0

#### Penyelesaian

```C++
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
        b;

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
```

#### Penjelasan

```C++
#define MATRIX_FACT_ROW 4
#define MATRIX_FACT_COL 6
#define MAX_THREAD 100

int matrix_fact[MATRIX_FACT_ROW][MATRIX_FACT_COL],
    (*matrix_multi_result)[MATRIX_FACT_COL];

unsigned long long int matrix_fact_result[MATRIX_FACT_ROW][MATRIX_FACT_COL];

typedef struct permutation_t
{
    int a,
        b;

    unsigned long long int *write_to;

} Permutation;
```

`matrix_fact` merupakan variabel untuk menyimpan matrix baru yang akan digunakan untuk melakukan perhitungan faktorial. `matrix_multi_result` merupakan shared memory yang menyimpan hasil perkalian yang telah di hasilkan oleh program 2A. `matrix_fact_result` menyimpan hasil perhitungan akhir dari faktorial sesuai dengan aturan yang diberikan.

Struct `Permutation` digunakan untuk melakukan passing argumen terhadap fungsi yang melakukan perhitungan. Dimana `a` dan `b` merupakan angka yang digunakan untuk melakukan perhitungan dan `write_to` digunakan untuk menulis data ke sebuah variabel yang bisa diambil oleh main program.

```C++
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
```

Diatas adalah fungsi utilitas yang dibuat untuk mendapatkan input matrix dan menampilkan hasil matrix.

```C++
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
```

Fungsi `factorial()` akan mengembalikan nilai factorial dari argumen yang diberikan. Dimana fungsi `factorial` akan dipanggil oleh fungsi `calculateCell()` yang merupakan sebuah fungsi yang nantinya akan berjalan sebagai sebuah thread. `calculateCell()` menerima sebuah argumen yang berbentuk `struct Permutation`. Dimana di dalamnya akan ada sejumlah percabangan untuk menentukan perhitungan yang cocok sesuai dengan syarat yang diberikan. Dan kemudian hasilnya di simpan ke sebuah memori yang ditunjuk oleh pointer `write_to`.

```C++

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
```

Fungsi `performmatrixCellPermutation` merupakan fungsi untuk melakukan iterasi ke semua titik dalam matrix. Disebut permutasi karena syarat yang diberikan mirip dengan rumus permutasi. Setiap node akan dibuatkan sebuah thread untuk dilakukan perhitungan dengan menggunakan fungsi `calculateCell` kemudian seluruh thread tersebut di join.

```C++
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
```

Diatas adalah main dari program. dimana ketika diajalankan akan mengambil terlebih dahulu hasil perkalian matrix dari program 2A. Kemudian akan meminta input matrix, dan melakukan perhitungan untuk masing masing node. Dan ketika sudah selesai perhitungganya maka akan ditampilkan.

### 2C. Membuat sebuah program untuk menampilkan 5 proses teratas yang memakan resource komputer.

#### Penyelesaian

```c++
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int first_pipe[2], second_pipe[2];

void execPs()
{
    dup2(first_pipe[1], 1);

    close(first_pipe[0]);
    close(first_pipe[1]);

    char *argv[] = {"ps", "aux", NULL};
    execv("/bin/ps", argv);
}

void execSort()
{
    dup2(first_pipe[0], 0);
    dup2(second_pipe[1], 1);

    close(first_pipe[0]);
    close(first_pipe[1]);
    close(second_pipe[0]);
    close(second_pipe[1]);

    char *argv[] = {"sort", "-nrk", "3,3", NULL};
    execv("/usr/bin/sort", argv);
}

void execHead()
{
    dup2(second_pipe[0], 0);

    close(second_pipe[0]);
    close(second_pipe[1]);

    char *argv[] = {"head", "-5", NULL};
    execv("/usr/bin/head", argv);
}

int main()
{
    if (pipe(first_pipe) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    if (fork() == 0)
    {
        execPs();
    }

    if (pipe(second_pipe) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    if (fork() == 0)
    {
        execSort();
    }

    close(first_pipe[0]);
    close(first_pipe[1]);

    if (fork() == 0)
    {
        execHead();
    }
}
```

#### Penejelasan

Program di mulai dengan membaut 2 buah pipe, yang nanti nya akan digunakan untuk passing hasil dari sebuah command untuk menjadi input bagi command selanjutnya. Setelah kedua pipe berhasil dibuat, maka program utama akan melakukan fork dimana child process akan menjalankan perintah `ps aux`. Dimana `first_pipe[1]` di assign sebagai stdout menggunakan perintah `dup2(first_pipe[1], 1);`

Setelah child process selesai, main program akan melakukan fork kembali dimana child process akan menjalankan perintah `sort -nrk 3,3`. Dimana `first_pipe[0]` di assign sebagai stdin menggunakan perintah `dup2(first_pipe[0], 0);` dan `second_pipe[1]` di assign sebagai stdout menggunakan perintah `dup2(second_pipe[1], 1);`

Setelah child process selesai, main program akan melakukan fork kembali dimana child process akan menjalankan perintah `head -5`. Dimana `second_pipe[0]` di assign sebagai stdin menggunakan perintah `dup2(second_pipe[0], 0);`

## NOMOR 3
