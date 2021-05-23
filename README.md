# soal-shift-sisop-modul-3-B02-2021

## NOMOR 1

### 1A. Register dan login

#### Penyelesaian

Server
####
```C++
void* login(){
    bzero(buffer, 1024);
    strcpy(buffer, "Enter Id:\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024);
    char id[199];
    strcpy(id, buffer);
    strcpy(buffer, "Enter Password:\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024);
    char pass[199];
    strcpy(pass, buffer);
    char acc[399];
    sprintf(acc, "%s:%s\n",id, pass);
    if(checkCreds(acc)>0){
        strcpy(buffer, "Login Success\n");
        send(sd, buffer, strlen(buffer), 0);
        status = 1;
        sprintf(user, "(%s:%s)", id, pass);
    }else{
        strcpy(buffer, "Wrong Id or Password. Please Try Again.\n");
        send(sd, buffer, strlen(buffer), 0);
    }
    bzero(buffer, 1024);
}  

void update(char msg[]) {
    FILE* file = fopen("akun.txt", "a") ;
    fputs(msg, file);
    fclose(file);
}

void* regist(){
    bzero(buffer, 1024);
    strcpy(buffer, "SignUp\nEnter New Id:\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024);
    char id[199];
    strcpy(id, buffer);
    strcpy(buffer, "Enter Password:\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024);
    char pass[199];
    strcpy(pass, buffer);

    char acc[399];
    sprintf(acc, "%s:%s\n",id, pass);
    update(acc);
    strcpy(buffer, "Registration Success. You may now login with your account.\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024) ;
}
```

Client
```C++
void* regist(){
    read(sock, buffer, 1024);
    printf("%s", buffer);
    bzero(buffer, 1024);
    scanf(" %s", buffer) ;
    send(sock, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sock, buffer, 1024);
    printf("%s", buffer);
    memset(buffer, 0, 1024) ;
    scanf(" %s", buffer) ;
    send(sock, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sock, buffer, 1024);
    printf("%s", buffer);
    if(strcmp(buffer, "Login Success\n")==0){
        printf("\n---Tools---\n\nadd\ndownload\ndelete\nsee\nfind\n\n");
        status=1;
    }
    bzero(buffer, 1024);
}
```

#### Penjelasan

Diberitaukan oleh soal bahwa ini merupakan multi connection socket, jadi saya memakai code dari https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/ untuk multi connectionnya. setelah tersambung dan mendapat input dari client dan diterima server, data itu akan disimpan di akun.txt dan saat login, file akun.txt akan dibaca dan di compare dengan input saat login. jika saat di compare sama maka akan keluar login success di server dan mengirim string tersebut ke client. saat cleint membaca login succes pada buffer, status akan menjadi 1 dan bisa mengakses fitur selanjutnya.

### 1B dan 1C. Buat folder FILES dan tools untuk menambahkan data (Add)

#### Penyelesaian

Server
```C++
void* add(){
    char pathFile[200], publisher[100], year[100], fileName[100];
    strcpy(buffer, "Publisher: \n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024) ;
    strcpy(publisher, buffer);
 
    strcpy(buffer, "Tahun Publish: \n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024) ;
 
    strcpy(year, buffer);
 
    strcpy(buffer, "Filepath: \n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024);
    strcpy(pathFile, buffer);
    strcpy(fileName, buffer);
    char temp[400];
    strcpy(temp, pathFile);
    strcat(temp, "\t");
    strcat(temp, publisher);
    strcat(temp, "\t");
    strcat(temp, year);
    
    char temp2[400];
    strcpy(temp2, serverPath);
    strcat(temp2, "FILES/");
    strcat(temp2, temp);
    strcat(temp2, "\n");
 
    FILE* file = fopen("files.tsv", "a");
    fputs(temp2, file);
    fclose(file);
 
    char path[50];
    strcpy(path, serverPath); 
    strcat(path, "FILES/"); 
    strcat(path, buffer);
 
    FILE* filee = fopen(path, "w") ;

    bzero(buffer, 1024);
    read(sd, buffer, 1024);
    fprintf(filee, "%s", buffer);
    fclose(filee);
    printf("[+]Data written in the file successfully.\n");


    bzero(buffer, 1024) ;
 
    strcpy(buffer, "Add Success\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
 
    FILE* log = fopen("running.log", "a");
    fprintf(log, "Tambah : %s %s\n", fileName, user);
    fclose(log);
}
```

Client
```C++
void* add(){
    bzero(buffer, 1024);
    read(sock, buffer, 1024) ;
    printf("%s", buffer);
    bzero(buffer, 1024);
    scanf(" %s", buffer) ;
    send(sock, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sock, buffer, 1024) ;
    printf("%s", buffer);
    bzero(buffer, 1024);
    scanf(" %s", buffer) ;
    send(sock, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sock, buffer, 1024) ;
    printf("%s", buffer);
    bzero(buffer, 1024);
    scanf(" %s", buffer);
    while(access(buffer, F_OK ) != 0){
        printf("File doesn't exist! Please try again.\n");
        bzero(buffer, 1024);
        scanf(" %s", buffer);
    }
    send(sock, buffer, strlen(buffer), 0);
     
    FILE *fp = fopen(buffer, "rb");
    if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }

    send_file(fp);
    printf("[+]File data sent successfully.\n");
    
    read(sock, buffer, 1024);
    printf("%s\n", buffer);
    bzero(buffer, 1024);
}
```

#### Penjelasan
ffolder FILES dibuat dengan meng cat path server dengan FILES/. lalu add akan menambahkan file yang sudah ada di folder client ke files.tsv dengan nama publisher dan tahun. 

### 1D. Mendownload file (Download)

#### Penyelesaian

Server
```C++
void *download(){
    if(strcmp(buffer, "download")==0){
        strcpy(buffer, "Enter file name!\n");
        send(sd, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
    }
    else{
        char *inputCopy = malloc(255 * sizeof(char));
        char *second = secondWord(buffer, inputCopy);

        int exist = 0;
        char path[400];
        strcpy(path, serverPath);
        strcat(path, "FILES/");
        strcat(path, second);
        char data[200];
        FILE *opfile = fopen("files.tsv", "rb");
        while(fgets(data, 200, opfile) != NULL) {
            char *input = malloc(255 * sizeof(char));
            char *first = firstWord(data, input);
            
            if(strcmp(first, path)==0) {
                exist = 1;
                break;
            }
            free(input);
            bzero(data, 200);
        }
        fclose(opfile);
        if(exist==0){
            strcpy(buffer, "File not found!\n");
            send(sd, buffer, strlen(buffer), 0);
            bzero(buffer, 1024);
        }
        else{
            char temp[200];
            strcpy(temp, "FILES/");
            strcat(temp, second);
            printf("%s\n", temp);
            int sz;
            FILE *fp = fopen(temp, "rb");
            if (fp == NULL) {
                perror("[-]Error in reading file.");
                exit(1);
            }
            
            while(fgets(data, 200, fp) != NULL) {
                if (send(sd, data, strlen(data), 0) == -1) {
                    perror("[-]Error in sending file.");
                    exit(1);
                }
                bzero(data, 200);
            }
            fclose(fp);
            printf("Filesent!\n");
        }
        free(inputCopy);
    }
}
```

Client
```C++
void *download(){
    char *inputCopy = malloc(255 * sizeof(char));
    char *second = secondWord(query, inputCopy);
    read(sock, buffer, 1024);
    if(strcmp(buffer, "Enter file name!\n")==0 || strcmp(buffer, "File not found!\n")==0){
        printf("%s", buffer);
    }
    else{
        char temp[300];
        strcpy(temp, clientPath);
        strcat(temp, second);
        // printf("%s\n", second);
        FILE* filee = fopen(temp, "w");
        fprintf(filee, "%s", buffer);
        fclose(filee);
        printf("File Download Success!\n");
    }
    free(inputCopy);
}
```

#### Penjelasan

download akan mengcopy file yang ada di folder files ke dalam folder client. di cek files.tsv lalu mengirim string nama dan isi files ke folder client dengan meng cat path client dengan nama file.


### 1E. Delete file lalu merubah data jadi old-NamaFile (Delete)

#### Penyelesaian

Server
```C++
void *delete(){
    if(strcmp(buffer, "delete")==0){
        strcpy(buffer, "Enter file name!\n");
        send(sd, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
    }else{
        char *inputCopy = malloc(255 * sizeof(char));
        char *second = secondWord(buffer, inputCopy);
        int exist = 0;
        char path[400];
        strcpy(path, serverPath);
        strcat(path, "FILES/");
        strcat(path, second);
        char data[400];
        FILE *opfile = fopen("files.tsv", "rb");
        FILE* filee = fopen("tmp.tsv", "w");
        char temp[500];
        while(fgets(data, 400, opfile)) {
            char *input = malloc(255 * sizeof(char));
            char *first = firstWord(data, input);
            if(strcmp(first, path)) {
                fprintf(filee, "%s", data);
            }
            free(input);
            bzero(data, 200);
        }
        fclose(opfile);
        fclose(filee);
        remove("files.tsv");
        rename("tmp.tsv", "files.tsv");

        char new[100] ;
        strcpy(new, serverPath);
        strcat(new, "FILES/old-");
        strcat(new, second);

        rename(path, new);

        strcpy(buffer, "Delete Success\n");
        send(sd, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
        FILE* log = fopen("running.log", "a") ;
        fprintf(log, "Hapus : %s %s\n", second, user) ;
        fclose(log) ;
        free(inputCopy);
    }
}
```

Client
```C++
void *delete(){
    read(sock, buffer, 1024);
    printf("%s", buffer);
}
```



### 1F. Melihat semua file (See)

#### Penyelesaian

Server
```C++
void *see(){
    FILE* file = fopen("files.tsv", "r");
    char line[1024];
    char publisher[255] = {0};
    char year[10] = {0};
    char path[511] = {0};
    char fileName[63] = {0};
    char name[50] = {0};
    char extension[10] = {0};
    char savePath[511] = {0};
    bzero(buffer, 1024);
    int i = 0;
    while(fgets(line, 1024, file)) {
        if (i) strcat(buffer, "\n\n");
        tab(line, path, publisher, year);
        strcpy(savePath, path);
        slash(path, fileName);
        dot(fileName, name, extension);
        strcat(buffer, "Nama: ");
        strcat(buffer, name);
        strcat(buffer, "\nPublisher: ");
        strcat(buffer, publisher);
        strcat(buffer, "\nTahun publishing: ");
        strcat(buffer, year);
        strcat(buffer, "\nEkstensi file: "); 
        strcat(buffer, extension) ;
        strcat(buffer, "\nFile path: ");
        strcat(buffer, savePath);
        i++;
    }
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
}
```

Client
```C++
void *see(){
    bzero(buffer, 1024) ;
    read(sock, buffer, 1024) ;
    printf("%s\n", buffer) ;
    bzero(buffer, 1024) ;
}
```

#### Penjelasan
see melihat tiap baris di files.tsv lalu menampilkannya ke format tertentu


### 1G. Mencari file (Find)

#### Penyelesaian
```C++
void *find(){
    if(strcmp(buffer, "find")==0) {
        strcpy(buffer, "Enter file name!\n");
        send(sd, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
    }
    else {
        char *inputCopy = malloc(255 * sizeof(char));
        char *second = secondWord(buffer, inputCopy);
        FILE* file = fopen("files.tsv", "r");
        char line[1024];
        char publisher[255] = {0};
        char year[10] = {0};
        char path[511] = {0};
        char fileName[63] = {0};
        char name[50] = {0};
        char extension[10] = {0};
        char savePath[511] = {0};
        bzero(buffer, 1024);
        
        int i = 0;
        while(fgets(line, 1024, file)) {
                
            tab(line, path, publisher, year);
            strcpy(savePath, path);
            slash(path, fileName);
            dot(fileName, name, extension);

            char searchName[63], searchExt[10];
            dot(second, searchName, searchExt);
            if (!strstr(name, searchName)) continue;

            if (i) 
                strcat(buffer, "\n\n");

            strcat(buffer, "Nama: ");
            strcat(buffer, name);
            strcat(buffer, "\nPublisher: "); 
            strcat(buffer, publisher);
            strcat(buffer, "\nTahun publishing: ");
            strcat(buffer, year);
            strcat(buffer, "\nEkstensi file: ");
            strcat(buffer, extension);
            strcat(buffer, "\nFile path: ");
            strcat(buffer, savePath);
            i++;
        }

        if (!i) {
            strcpy(buffer, "File not found\n");
        }
        send(sd, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
        free(inputCopy);
    }
}
```

#### Penjelasan
find akan mencari input ke dalam files.tv. jika ada akan dikeluarkan seperti format see


### 1H. buat running.log

#### Penyelesaian
```C++
FILE* log = fopen("running.log", "a");
  fprintf(log, "activity: %s %s\n", fileName, user);
  fclose(log);
```

#### Penjelasan
pada tiap fitur ditambahkan fungsi diatas dengan activity sesuai fiturnya

### Source code

#### Server
```C++
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/stat.h>
#include <netinet/in.h> 
#include <sys/time.h>
#define PORT 8080
#define MAX 80
int server_fd, new_socket, valread, sdMax;
int master_socket , addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd;  
int max_sd;  
int someoneConnected, status;
char buffer[1024];
char user[1024];
char serverPath[] = "/home/arkan/Documents/Modul 3/soal-shift-sisop-modul-3-B02-2021-/soal 1/Server/";
void swapQueue() {
    int i ;
    for (i = 1 ; i < max_clients ; i++) {
        int temp = client_socket[i];
        client_socket[i] = client_socket[i-1];
        client_socket[i-1] = temp;
    }
}

int checkCreds(char cred[]){
    FILE* file = fopen("akun.txt", "r");
    char check[99];
    while(fgets(check, 99, file)) {
        if (!strcmp(check, cred)) {
            fclose(file) ;
            return 1 ;
        }
    }
    fclose(file) ;
    return 0 ;
}

void* login(){
    bzero(buffer, 1024);
    strcpy(buffer, "Enter Id:\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024);
    char id[199];
    strcpy(id, buffer);
    strcpy(buffer, "Enter Password:\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024);
    char pass[199];
    strcpy(pass, buffer);
    char acc[399];
    sprintf(acc, "%s:%s\n",id, pass);
    if(checkCreds(acc)>0){
        strcpy(buffer, "Login Success\n");
        send(sd, buffer, strlen(buffer), 0);
        status = 1;
        sprintf(user, "(%s:%s)", id, pass);
    }else{
        strcpy(buffer, "Wrong Id or Password. Please Try Again.\n");
        send(sd, buffer, strlen(buffer), 0);
    }
    bzero(buffer, 1024);
}  

void update(char msg[]) {
    FILE* file = fopen("akun.txt", "a") ;
    fputs(msg, file);
    fclose(file);
}

void* regist(){
    bzero(buffer, 1024);
    strcpy(buffer, "SignUp\nEnter New Id:\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024);
    char id[199];
    strcpy(id, buffer);
    strcpy(buffer, "Enter Password:\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024);
    char pass[199];
    strcpy(pass, buffer);

    char acc[399];
    sprintf(acc, "%s:%s\n",id, pass);
    update(acc);
    strcpy(buffer, "Registration Success. You may now login with your account.\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024) ;
}

void* err(){
    strcpy(buffer, "Invalid Command\n") ;
    send(sd, buffer, strlen(buffer), 0) ;
    bzero(buffer, 1024);
}

void isSafe(){
     if ((valread = read( sd , buffer, 1024)) == 0)  
        {                  
            close( sd );
            someoneConnected = 0;
            status = 0;
            client_socket[i] = 0;
            swapQueue();
        }
}

void write_file(char path[]){
  int n;
  FILE *fp;

  fp = fopen(path, "w");
  while (1) {
    n = recv(sd, buffer, 1024, 0);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, 1024);
  }
  return;
}

void* add(){
    char pathFile[200], publisher[100], year[100], fileName[100];
    strcpy(buffer, "Publisher: \n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024) ;
    strcpy(publisher, buffer);
 
    strcpy(buffer, "Tahun Publish: \n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024) ;
 
    strcpy(year, buffer);
 
    strcpy(buffer, "Filepath: \n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024);
    strcpy(pathFile, buffer);
    strcpy(fileName, buffer);
    char temp[400];
    strcpy(temp, pathFile);
    strcat(temp, "\t");
    strcat(temp, publisher);
    strcat(temp, "\t");
    strcat(temp, year);
    
    char temp2[400];
    strcpy(temp2, serverPath);
    strcat(temp2, "FILES/");
    strcat(temp2, temp);
    strcat(temp2, "\n");
 
    FILE* file = fopen("files.tsv", "a");
    fputs(temp2, file);
    fclose(file);
 
    char path[50];
    strcpy(path, serverPath); 
    strcat(path, "FILES/"); 
    strcat(path, buffer);
 
    FILE* filee = fopen(path, "w") ;

    bzero(buffer, 1024);
    read(sd, buffer, 1024);
    fprintf(filee, "%s", buffer);
    fclose(filee);
    printf("[+]Data written in the file successfully.\n");


    bzero(buffer, 1024) ;
 
    strcpy(buffer, "Add Success\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
 
    FILE* log = fopen("running.log", "a");
    fprintf(log, "Tambah : %s %s\n", fileName, user);
    fclose(log);
}

char* secondWord(char str[], char inputCpy[]){
    const char del[10] = " ";
    strcpy(inputCpy, str);
    char * ptr = strtok(inputCpy, del);    
    return strtok(NULL, del); 
}
char* firstWord(char str[], char inputCpy[]){
    char del[] = "\t";
    strcpy(inputCpy, str);
    return strtok(inputCpy, del);  
}

void *download(){
    if(strcmp(buffer, "download")==0){
        strcpy(buffer, "Enter file name!\n");
        send(sd, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
    }
    else{
        char *inputCopy = malloc(255 * sizeof(char));
        char *second = secondWord(buffer, inputCopy);

        int exist = 0;
        char path[400];
        strcpy(path, serverPath);
        strcat(path, "FILES/");
        strcat(path, second);
        char data[200];
        FILE *opfile = fopen("files.tsv", "rb");
        while(fgets(data, 200, opfile) != NULL) {
            char *input = malloc(255 * sizeof(char));
            char *first = firstWord(data, input);
            
            if(strcmp(first, path)==0) {
                exist = 1;
                break;
            }
            free(input);
            bzero(data, 200);
        }
        fclose(opfile);
        if(exist==0){
            strcpy(buffer, "File not found!\n");
            send(sd, buffer, strlen(buffer), 0);
            bzero(buffer, 1024);
        }
        else{
            char temp[200];
            strcpy(temp, "FILES/");
            strcat(temp, second);
            printf("%s\n", temp);
            int sz;
            FILE *fp = fopen(temp, "rb");
            if (fp == NULL) {
                perror("[-]Error in reading file.");
                exit(1);
            }
            
            while(fgets(data, 200, fp) != NULL) {
                if (send(sd, data, strlen(data), 0) == -1) {
                    perror("[-]Error in sending file.");
                    exit(1);
                }
                bzero(data, 200);
            }
            fclose(fp);
            printf("Filesent!\n");
        }
        free(inputCopy);
    }
}

void *delete(){
    if(strcmp(buffer, "delete")==0){
        strcpy(buffer, "Enter file name!\n");
        send(sd, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
    }else{
        char *inputCopy = malloc(255 * sizeof(char));
        char *second = secondWord(buffer, inputCopy);
        int exist = 0;
        char path[400];
        strcpy(path, serverPath);
        strcat(path, "FILES/");
        strcat(path, second);
        char data[400];
        FILE *opfile = fopen("files.tsv", "rb");
        FILE* filee = fopen("tmp.tsv", "w");
        char temp[500];
        while(fgets(data, 400, opfile)) {
            char *input = malloc(255 * sizeof(char));
            char *first = firstWord(data, input);
            if(strcmp(first, path)) {
                fprintf(filee, "%s", data);
            }
            free(input);
            bzero(data, 200);
        }
        fclose(opfile);
        fclose(filee);
        remove("files.tsv");
        rename("tmp.tsv", "files.tsv");

        char new[100] ;
        strcpy(new, serverPath);
        strcat(new, "FILES/old-");
        strcat(new, second);

        rename(path, new);

        strcpy(buffer, "Delete Success\n");
        send(sd, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
        FILE* log = fopen("running.log", "a") ;
        fprintf(log, "Hapus : %s %s\n", second, user) ;
        fclose(log) ;
        free(inputCopy);
    }
}
void tab(char msg[], char a[], char b[], char c[]) {
    int i = 0 ;
    char* check ;
    while (check = strtok_r(msg, "\t", &msg)) {
        if (i == 0) strcpy(a, check) ;
        else if (i == 1) strcpy(b, check) ;
        else if (i == 2) strncpy(c, check, strlen(check) - 1) ;
        i++ ;
    }
}

void slash(char msg[], char a[]) {
    int i = 0 ;
    char* check ;
    while (check = strtok_r(msg, "/", &msg)) {
        strcpy(a, check) ;
        i++ ;
    }
}

void dot(char msg[], char a[], char b[]) {
    int i = 0 ;
    char* check ;
    if (strchr(msg, '.')) {
        char* ext = strchr(msg, '.') + 1 ;
        strcpy(b, ext) ;
    }
    while (check = strtok_r(msg, ".", &msg)) {
        if (i == 0) strcpy(a, check) ;
        
        i++ ;
    }
}

void *see(){
    FILE* file = fopen("files.tsv", "r");
    char line[1024];
    char publisher[255] = {0};
    char year[10] = {0};
    char path[511] = {0};
    char fileName[63] = {0};
    char name[50] = {0};
    char extension[10] = {0};
    char savePath[511] = {0};
    bzero(buffer, 1024);
    int i = 0;
    while(fgets(line, 1024, file)) {
        if (i) strcat(buffer, "\n\n");
        tab(line, path, publisher, year);
        strcpy(savePath, path);
        slash(path, fileName);
        dot(fileName, name, extension);
        strcat(buffer, "Nama: ");
        strcat(buffer, name);
        strcat(buffer, "\nPublisher: ");
        strcat(buffer, publisher);
        strcat(buffer, "\nTahun publishing: ");
        strcat(buffer, year);
        strcat(buffer, "\nEkstensi file: "); 
        strcat(buffer, extension) ;
        strcat(buffer, "\nFile path: ");
        strcat(buffer, savePath);
        i++;
    }
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
}
void *find(){
    if(strcmp(buffer, "find")==0) {
        strcpy(buffer, "Enter file name!\n");
        send(sd, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
    }
    else {
        char *inputCopy = malloc(255 * sizeof(char));
        char *second = secondWord(buffer, inputCopy);
        FILE* file = fopen("files.tsv", "r");
        char line[1024];
        char publisher[255] = {0};
        char year[10] = {0};
        char path[511] = {0};
        char fileName[63] = {0};
        char name[50] = {0};
        char extension[10] = {0};
        char savePath[511] = {0};
        bzero(buffer, 1024);
        
        int i = 0;
        while(fgets(line, 1024, file)) {
                
            tab(line, path, publisher, year);
            strcpy(savePath, path);
            slash(path, fileName);
            dot(fileName, name, extension);

            char searchName[63], searchExt[10];
            dot(second, searchName, searchExt);
            if (!strstr(name, searchName)) continue;

            if (i) 
                strcat(buffer, "\n\n");

            strcat(buffer, "Nama: ");
            strcat(buffer, name);
            strcat(buffer, "\nPublisher: "); 
            strcat(buffer, publisher);
            strcat(buffer, "\nTahun publishing: ");
            strcat(buffer, year);
            strcat(buffer, "\nEkstensi file: ");
            strcat(buffer, extension);
            strcat(buffer, "\nFile path: ");
            strcat(buffer, savePath);
            i++;
        }

        if (!i) {
            strcpy(buffer, "File not found\n");
        }
        send(sd, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
        free(inputCopy);
    }
}

int main(int argc, char const *argv[]) {

    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    fd_set readfds;
    //initialise all client_socket[] to 0 so not checked 
    for (i = 0; i < max_clients; i++)  
    {  
        client_socket[i] = 0;  
    }  
      
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
    {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
     
    //set master socket to allow multiple connections , 
    //this is just a good habit, it will work without this 
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
          sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
     
    //type of socket created 
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( PORT );  
         
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
    {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
    printf("Listener on port %d \n", PORT);  
         
    //try to specify maximum of 3 pending connections for the master socket 
    if (listen(master_socket, 3) < 0)  
    {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }  
         
    //accept the incoming connection 
    addrlen = sizeof(address);  
    puts("Waiting for connections ...");  
         
    FILE *outfile = fopen("akun.txt", "ab+");
    FILE *fp = fopen("files.tsv", "ab+");
    mkdir("FILES",0777);
    fclose(outfile);
    for(;;){

        FD_ZERO(&readfds);  
     

        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;  
             

        for ( i = 0 ; i < max_clients ; i++)  
        {  
            sd = client_socket[i];  
                 

            if(sd > 0)  
                FD_SET( sd , &readfds);  
            if(sd > max_sd)  
                max_sd = sd;  
        }  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
       
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        }  
        if (FD_ISSET(master_socket, &readfds))  
        {  
            if ((new_socket = accept(master_socket, 
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
            {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }
            
            char *message = "Connected..\n";

            //send new connection greeting message 
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )  
            {  
                perror("Error Sending Message\n");  
            }
                 
            //add new socket to array of sockets 
            for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    break;  
                }  
            }  
        }
             
        sd = client_socket[i];
        someoneConnected = 1;
        while(someoneConnected>0){
            isSafe();
            pthread_t thread;
            if(strcmp(buffer, "1")==0){
                pthread_create(&(thread),NULL,&regist,NULL);
            }else if(strcmp(buffer, "2")==0){
                pthread_create(&(thread),NULL,&login,NULL);
            }else if(status==0){
                pthread_create(&(thread),NULL,&err,NULL);
            }else{
                if(strcmp(buffer, "add")==0){
                    pthread_create(&(thread),NULL,&add,NULL);
                }else if(strncmp(buffer, "download", 8)==0){
                    pthread_create(&(thread),NULL,&download,NULL);
                }else if(strncmp(buffer, "delete", 6)==0){
                    pthread_create(&(thread),NULL,&delete,NULL);
                }else if(strcmp(buffer, "see")==0){
                    pthread_create(&(thread),NULL,&see,NULL);
                }else if(strncmp(buffer, "find", 4)==0){
                    pthread_create(&(thread),NULL,&find,NULL);
                }else{
                    pthread_create(&(thread),NULL,&err,NULL);
                }
            }
            pthread_join(thread, NULL);
        }
    }

    close(server_fd);
    return 0;
}
```

#### Client
```C++
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/stat.h>
#include <netinet/in.h> 
#include <sys/time.h>
#include <string.h>
#define PORT 8080
int sock = 0, valread;
int status;
char query[80] = {0};
pthread_t thread;
char buffer[1024];
struct sockaddr_in serv_addr;
int wait=0;
char clientPath[] = "/home/arkan/Documents/Modul 3/soal-shift-sisop-modul-3-B02-2021-/soal 1/Client/";

void trimTrailing(char * str)
{
    int index, i;

    /* Set default index */
    index = -1;

    /* Find last index of non-white space character */
    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            index= i;
        }

        i++;
    }

    /* Mark next character to last non-white space character as NULL */
    str[index + 1] = '\0';
}

void* regist(){
    read(sock, buffer, 1024);
    printf("%s", buffer);
    bzero(buffer, 1024);
    scanf(" %s", buffer) ;
    send(sock, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sock, buffer, 1024);
    printf("%s", buffer);
    memset(buffer, 0, 1024) ;
    scanf(" %s", buffer) ;
    send(sock, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sock, buffer, 1024);
    printf("%s", buffer);
    if(strcmp(buffer, "Login Success\n")==0){
        printf("\n---Tools---\n\nadd\ndownload\ndelete\nsee\nfind\n\n");
        status=1;
    }
    bzero(buffer, 1024);
}

void* err(){
    read(sock, buffer, 1024);
    printf("%s\n", buffer);
    bzero(buffer, 1024);
}

void send_file(FILE *fp){
  int n;
  char data[200] = {0};

  while(fgets(data, 200, fp) != NULL) {
    if (send(sock, data, strlen(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, 200);
  }
  fclose(fp) ;
}

void* add(){
    bzero(buffer, 1024);
    read(sock, buffer, 1024) ;
    printf("%s", buffer);
    bzero(buffer, 1024);
    scanf(" %s", buffer) ;
    send(sock, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sock, buffer, 1024) ;
    printf("%s", buffer);
    bzero(buffer, 1024);
    scanf(" %s", buffer) ;
    send(sock, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sock, buffer, 1024) ;
    printf("%s", buffer);
    bzero(buffer, 1024);
    scanf(" %s", buffer);
    while(access(buffer, F_OK ) != 0){
        printf("File doesn't exist! Please try again.\n");
        bzero(buffer, 1024);
        scanf(" %s", buffer);
    }
    send(sock, buffer, strlen(buffer), 0);
     
    FILE *fp = fopen(buffer, "rb");
    if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }

    send_file(fp);
    printf("[+]File data sent successfully.\n");
    
    read(sock, buffer, 1024);
    printf("%s\n", buffer);
    bzero(buffer, 1024);
}

char* secondWord(char str[], char inputCpy[]){
    const char del[10] = " ";
    strcpy(inputCpy, str);
    char * ptr = strtok(inputCpy, del);
    return strtok(NULL, del); 
}

void *download(){
    char *inputCopy = malloc(255 * sizeof(char));
    char *second = secondWord(query, inputCopy);
    read(sock, buffer, 1024);
    if(strcmp(buffer, "Enter file name!\n")==0 || strcmp(buffer, "File not found!\n")==0){
        printf("%s", buffer);
    }
    else{
        char temp[300];
        strcpy(temp, clientPath);
        strcat(temp, second);
        // printf("%s\n", second);
        FILE* filee = fopen(temp, "w");
        fprintf(filee, "%s", buffer);
        fclose(filee);
        printf("File Download Success!\n");
    }
    free(inputCopy);
}

void *delete(){
    read(sock, buffer, 1024);
    printf("%s", buffer);
}

void *see(){
    bzero(buffer, 1024) ;
    read(sock, buffer, 1024) ;
    printf("%s\n", buffer) ;
    bzero(buffer, 1024) ;
}

void *find(){
    bzero(buffer, 1024) ;
    read(sock, buffer, 1024) ;
    printf("%s\n", buffer) ;
    bzero(buffer, 1024) ;
}

int main(int argc, char const *argv[]) {
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    read(sock, buffer, 1024) ;
    printf("%s\n", buffer) ;
    bzero(buffer, 1024);
    printf("Main Menu\n1.Register\n2.Login\n");
    for(;;){
        printf("input: ");
        scanf(" %[^\n]", query);
        trimTrailing(query);
        send(sock , query , strlen(query) , 0 );
        if(strcmp(query, "1")==0){
            pthread_create(&(thread),NULL,&regist,NULL);
        }else if(strcmp(query, "2")==0){
            pthread_create(&(thread),NULL,&regist,NULL);
        }else if(status==0){
            pthread_create(&(thread),NULL,&err,NULL);
        }else{
            printf("\nTools\n\nadd\ndownload\ndelete\nsee\nfind\n\n");
            if(strcmp(query, "add")==0){
                pthread_create(&(thread),NULL,&add,NULL);
            }else if(strncmp(query, "download", 8)==0){
                pthread_create(&(thread),NULL,&download,NULL);
            }else if(strncmp(query, "delete", 6)==0){
                pthread_create(&(thread),NULL,&delete,NULL);
            }else if(strcmp(query, "see")==0){
                pthread_create(&(thread),NULL,&see,NULL);
            }else if(strncmp(query, "find", 4)==0){
                pthread_create(&(thread),NULL,&find,NULL);
            }else{
                pthread_create(&(thread),NULL,&err,NULL);
            }
        }
        pthread_join(thread, NULL);
    }
    close(sock);
    return 0;
}
```

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

#### Penyelesaian
```C++
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
```

#### Penjelasan
Pada bagian fungsi cekExist() digunakan untuk mengecek apakah file tersedia atau tidak pada directory yang sedang dituju.
```C++
int cekExist(const char *filenamae)
{
    struct stat buffer;
    int exist = stat(filenamae, &buffer);
    if (exist == 0)
        return 1;
    else
        return 0;
}
```

Untuk fungsi move() digunakan untuk move file ke dalam directory-directory atau folder-folder yang sesuai dengan ekstensi file tersebut. Dan jika file nya hidden, maka akan dimasukkan kedalam directory yang bernama Hidden.
```C++
if (hidden[1] == '.')
    {
        strcpy(dirnamae, "Hidden");
    }
```

Dan juga, jika ada file yang tidak memiliki ekstensi maka akan dimasukkan ke folder Unknown.
```C++
 //file gaada extensi
    else
    {
        strcpy(dirnamae, "Unknown");
    }
```

Untuk move file menggunakan ```rename(filenamae, namaefile)```


Untuk penggunaan opsi ```-f``` dimana kegunaannya sama dengan opsi ```-d``` tetapi dengan opsi ```-f``` ini mengkategorikan file. Berbeda dengan opsi ```-d``` yang mana dapat mengkategorikan hanya 1 directory per dijalankan, opsi ```-f``` dapat mengkategorikan beberapa file dalam sekali jalan.
```C++
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
```
Untuk menjalankan opsi ```-f``` yaitu
```shell
$ ./soal3 -f /path/to/file1.zip path/to/file2.c path/to/file3.TXT
```
untuk cara diatas, pada bagian .zip, .c, dan .TXT merupakan ekstensi file. Jadi bisa menggunakan ekstensi yang lain.

Untuk fungsi rekursifList() digunakan pada opsi ```-d``` dan opsi ```*```.

Jika menggunakan opsi ```-d``` maka akan menjalankan path/to/directory nya kedalam fungsi tersebut.
Directory tersebut akan di kategorikan.
```C++
  if (strcmp(argv[1], "*") == 0)

        {

            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {

                //menggunakan rekursif untuk mengkategorikan file di working directory
                rekursifList(cwd);

            }

        }
```
Jika berhasil maka akan mengeluarkan  **“Direktori sukses disimpan!”** dan jika gagal **“Yah, gagal disimpan :(“**.
Untuk menjalankan opsi ```-d``` pada terminal yaitu
```shell
$ ./soal3 -d /path/to/directory
```

Untuk penggunaan opsi terakhir yaitu opsi ```*``` sendiri ini berlaku beda dengan opsi-opsi yang lain. Dimana opsi ```-d``` untuk mengkategorikan directory dan opsi ```-f``` akan mengkategorikan file yang tersedia. Untuk opsi ```*``` ini, jika kita memanggilnya di terminal, maka file-file yang ada di directory tersebut akan di masukkan kedalam directory-directory masing-masing sesuai dengan ekstensi file tersebut.
```C++
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
```
Untuk memanggil opsi ```*``` yaitu
```shell
$ ./soal3 \*
```
