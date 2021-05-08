#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h>
#include <stdbool.h>
#include <pthread.h>
#define PORT 8888
int opt=1;
int master_socket , addrlen , new_socket , client_socket[30] , 
          max_clients = 30 , activity, i , valread , sd;  
int max_sd;  
struct sockaddr_in address; 
int anyconnected, anylogged;

char buffer[1025];
char uname[99];char pass[99];
int ada;

void mundursatu() {
    int i ;
    for (i = 1 ; i < max_clients ; i++) {
        client_socket[i - 1] = client_socket[i] + client_socket[i - 1] - (client_socket[i] = client_socket[i - 1]) ;
    }
}

void saferead() {
    if ((valread = read( sd , buffer, 1024)) == 0){  
        close( sd );  
        client_socket[i] = 0;
        anylogged = 0 ; anyconnected = 0 ;
        mundursatu() ;  
    }
}

void* daftar(){
    bzero(buffer,1024);
    strcpy(buffer, "Masukkan username: ");
    send(sd,buffer,strlen(buffer),0);
    bzero(buffer,1024);
    read(sd,buffer,1024);
    strcpy(uname,buffer);
    bzero(buffer,1024);

    strcpy(buffer,"Masukkan password: ");
    send(sd,buffer,strlen(buffer),0);
    bzero(buffer,1024);
    read(sd,buffer,1024);
    
    strcpy(pass,buffer);
    strcpy(buffer, "\nAkun anda terdaftar.\n\n");
    send(sd,buffer,strlen(buffer),0);
    bzero(buffer,1024);

    strcpy(buffer, "main menu\n 1.register\n 2.login\n\n");
    send(sd,buffer,strlen(buffer),0);
    bzero(buffer,1024);

    char temp[199];
    FILE* uye1=fopen("akun.txt", "a");
    fprintf(uye1, "%s:%s\n", uname, pass);
    // fputs(temp, uye);
    fclose(uye1);
    // printf("tolonglahhh");
}

bool cekeuy(char yuk[]){
    FILE* uye=fopen("akun.txt", "r");
    char cek[99];
    while(fgets(cek, 99, uye)){
        if(!strcmp(cek, yuk)){
            return true;
            fclose(uye);
        }
    }
    fclose(uye);
    return false;
}

void *masok(void*arg){
    char cekname[99], cekpass[99];
    char cek[199];
    bzero(buffer,1024);
    strcpy(buffer, "Masukkan username: ");
    send(sd,buffer,strlen(buffer),0);
    bzero(buffer,1024);
    read(sd,buffer,1024);
    strcpy(cekname,buffer);
    bzero(buffer,1024);
    strcpy(buffer,"Masukkan password: ");
    send(sd,buffer,strlen(buffer),0);
    bzero(buffer,1024);
    read(sd,buffer,1024);
    strcpy(cekpass,buffer);
    sprintf(cek, "%s:%s\n", cekname, cekpass);

    if(cekeuy(cek)==true){
        strcpy(buffer, "\nberhasil\nketik 0 untuk melanjutkan\n\n");
        send(sd,buffer,strlen(buffer),0);
        ada=1;
    }
    
    else{
        strcpy(buffer, "\ngagal login\n\n");
        send(sd,buffer,strlen(buffer),0);
        ada=0;
    }
    bzero(buffer, 1024);
}

void* tools(){
    char path[99], publisher[99], tahun[99];

    // bzero(buffer,1024);
    // strcpy(buffer, "1.tambah\n2.kurang\n3.exit");
    // send(sd,buffer,strlen(buffer),0);

    // bzero(buffer,1024);
    // strcpy(buffer, "Masukkan username\n");
    // send(sd,buffer,strlen(buffer),0);
    // bzero(buffer,1024);
    // read(sd,buffer,1024);

    // if(!strcmp(buffer, "1")){
        
    strcpy(buffer, "Publisher: \n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024) ;
    strcpy(publisher, buffer);
    bzero(buffer, 1024);

    strcpy(buffer, "Tahun Publish: \n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024) ;
    strcpy(tahun, buffer);
    bzero(buffer, 1024);

    strcpy(buffer, "Filepath: \n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sd, buffer, 1024) ;
    strcpy(path, buffer);
    char temp[199];
    strcpy(temp, path);
    strcat(temp, "\t");
    strcat(temp, publisher);
    strcat(temp, "\t");
    strcat(temp, tahun);
    
    char temp2[199];
    strcpy(temp2, "/home/arkan/Modul3/Server/");
    strcat(temp2, "FILES/");
    strcat(temp2, temp);
    strcat(temp2, "\n");
 
    FILE* uye = fopen("files.tsv", "a");
    fputs(temp2, uye);
    fclose(uye);
 
    char pathh[50];
    strcpy(pathh, "/home/arkan/Modul3/Server/"); 
    strcat(pathh, "FILES/"); 
    strcat(pathh, buffer);
 
    FILE* aye = fopen(path, "w") ;
    fclose(aye);

    bzero(buffer, 1024) ;
    strcpy(buffer, "berhasil ditambahkan\n");
    send(sd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    // }
}

void* error(){
    if (valread == 0) return 0;
}

int main(){
    fd_set readfds;

    // char* message="daimon dor dor v69\r\n";

    for(i=0;i<max_clients;i++){
        client_socket[i]=0;
    }

    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
    {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  

    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
          sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  

    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( PORT );

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
    {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
    printf("Listener on port %d \n", PORT);

    if (listen(master_socket, 3) < 0)  
    {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }

    addrlen = sizeof(address);  
    puts("Waiting for connections ...");

    while(1)  
    {  
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
             
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
           
            // if( send(new_socket, message, strlen(message), 0) != strlen(message) )  
            // {  
            //     perror("send");  
            // }  
                 
            // puts("Welcome message sent successfully");  
                 
            for (i = 0; i < max_clients; i++)  
            {  
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                         
                    break;  
                }  
            }  
        }

        // for (i = 0; i < max_clients; i++)  
        // {  
        //     sd = client_socket[i];  
                 
        //     if (FD_ISSET( sd , &readfds))  
        //     {  
        //         if ((valread = read( sd , buffer, 1024)) == 0)  
        //         {  
        //             getpeername(sd , (struct sockaddr*)&address , \
        //                 (socklen_t*)&addrlen);  
        //             printf("Host disconnected , ip %s , port %d \n" , 
        //                   inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                         
        //             close( sd );  
        //             client_socket[i] = 0;  
        //         }  
        
        //         else 
        //         { 
        //             buffer[valread] = '\0';  
        //             send(sd , buffer , strlen(buffer) , 0 );
        //         }
        //     }  
        // } 

        sd=client_socket[i];
        anyconnected=1;
        while(anyconnected){
            pthread_t thread;
            saferead();
            if(strcmp(buffer,"1")==0){
                // printf("asu");
                pthread_create(&(thread), NULL, &daftar, NULL);
            }
            else if(strcmp(buffer,"2")==0){
                pthread_create(&(thread), NULL, &masok, NULL);
                // if(ada==1) aaa=ada;
                // printf("MENU ASOYY\n");
            }

            // else if(ada==0) continue;

            else if(ada==1){
                // printf("MENU ASOYY\n");
                pthread_create(&(thread), NULL, &tools, NULL);
            }

            // else{
            //     pthread_create(&(thread), NULL, &error, NULL);
            // }

            pthread_join(thread,NULL);
        }




    }  
         
    return 0;  

}