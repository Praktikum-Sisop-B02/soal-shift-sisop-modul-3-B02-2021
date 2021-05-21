#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>

//fungsi buat cek eksis file
int cekExist(const char *filenamae)
{
    struct stat buffer;
    int exist = stat(filenamae, &buffer);
    if (exist == 0)
        return 1;
    else
        return 0;
}

void *move(void *filenamae)
{
    char cwd[PATH_MAX];
    char dirnamae[200], hidden[100], hiddennamae[100], file[100], existfile[100];
    int i;

    strcpy(existfile, filenamae);
    strcpy(hiddennamae, filenamae);

    char *namae = strrchr(hiddennamae, '/');
    strcpy(hidden, namae);

    //Untuk file yang hidden, awalan '.'
    if (hidden[1] == '.')
    {
        strcpy(dirnamae, "Hidden");
    }
    //File biasa
    else if (strstr(filenamae, ".") != NULL)
    {
        strcpy(file, filenamae);
        strtok(file, ".");
        char *token = strtok(NULL, "");
        //nggak case sensitive
        for (i = 0; token[i]; i++)
        {
            token[i] = tolower(token[i]);
        }
        strcpy(dirnamae, token);
    }
    //file gaada extensi
    else
    {
        strcpy(dirnamae, "Unknown");
    }

    //cek file ada ato ga, kalo ga dibuat folder
    int exist = cekExist(existfile);
    if (exist)
        mkdir(dirnamae, 0755);

    //dapet nama file
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {

        char *namae = strrchr(filenamae, '/');
        char namaefile[200];

        strcpy(namaefile, cwd);
        strcat(namaefile, "/");
        strcat(namaefile, dirnamae);
        strcat(namaefile, namae);

        //move file pake rename
        rename(filenamae, namaefile);
    }
}

//rekursif list
void rekursifList(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    struct stat buffer;
    DIR *dir = opendir(basePath);
    int n = 0;

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {

            // membuat new path dengan base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            if (stat(path, &buffer) == 0 && S_ISREG(buffer.st_mode))
            {

                //membuat thread untuk cek move
                pthread_t thread;
                int err = pthread_create(&thread, NULL, move, (void *)path);
                pthread_join(thread, NULL);

            }

            rekursifList(path);

        }

    }

    closedir(dir);

}

int main(int argc, char *argv[])
{

    char cwd[PATH_MAX];

    //Ketika ada argv[1] nya -f
    //./soal3 -f
    if (strcmp(argv[1], "-f") == 0)
    {

        pthread_t thread;
        int i;

        //buat thread dan print msg
        for (i = 2; i < argc; i++)
        {

            char printmsg[1000];
            int exist = cekExist(argv[i]);

            if (exist)
            {

                sprintf(printmsg, "File %d : Berhasil Dikategorikan", i - 1);

            }

            else

            {

                sprintf(printmsg, "File %d : Sad, gagal :(", i - 1);

            }

            printf("%s\n", printmsg);
            int err = pthread_create(&thread, NULL, move, (void *)argv[i]);

        }

        //join semua thread
        pthread_join(thread, NULL);

    }

    else

    {

        //ketika argv[1] nya *
        //./soal3 \*
        if (strcmp(argv[1], "*") == 0)

        {

            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {

                //menggunakan rekursif untuk mengkategorikan file di working directory
                rekursifList(cwd);

            }

        }

        //ketika ada argv[1] nya -d
        else if (strcmp(argv[1], "-d") == 0)
        {

            //membuka direktori sesuai argument kedua
            //./soal3 -d path/to/directory
            rekursifList(argv[2]);
            struct stat buffer;
            int err = stat(argv[2], &buffer);

            if (err == -1)
            {

                printf("Yah, gagal disimpan :(\n");

            }

            else

            {

                printf("Direktori sukses disimpan!\n");

            }

        }

    }

}
