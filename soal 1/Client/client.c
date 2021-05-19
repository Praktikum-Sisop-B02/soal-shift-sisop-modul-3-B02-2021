#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define PORT 8888

int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};

void *regis(){
    bzero(buffer,1024);
    read(sock,buffer,1024);
    printf("%s", buffer);
    bzero(buffer,1024);
    scanf("%s", buffer);
    send(sock,buffer,strlen(buffer),0);

    bzero(buffer,1024);
    read(sock,buffer,1024);
    printf("%s", buffer);
    bzero(buffer,1024);
    scanf("%s", buffer);
    send(sock,buffer,strlen(buffer),0);
    read(sock,buffer,1024);
    printf("%s", buffer);
    bzero(buffer,1024);
    // read(sock,buffer,1024);
    // printf("%s", buffer);
    // bzero(buffer,1024);
    // printf("asu");
}

void *login(){
    bzero(buffer,1024);
    read(sock,buffer,1024);
    printf("%s", buffer);
    bzero(buffer,1024);
    scanf("%s", buffer);
    send(sock,buffer,strlen(buffer),0);

    bzero(buffer,1024);
    read(sock,buffer,1024);
    printf("%s", buffer);
    bzero(buffer,1024);
    scanf("%s", buffer);
    send(sock,buffer,strlen(buffer),0);
    read(sock,buffer,1024);
    printf("%s", buffer);
}

void *add(){
    bzero(buffer,1024);
    read(sock,buffer,1024);
    printf("%s", buffer);

    bzero(buffer,1024);
    scanf("%s", buffer);
    send(sock,buffer,strlen(buffer),0);

    bzero(buffer,1024);
    read(sock,buffer,1024);
    printf("%s", buffer);

    bzero(buffer,1024);
    scanf("%s", buffer);
    send(sock,buffer,strlen(buffer),0);

    bzero(buffer,1024);
    read(sock,buffer,1024);
    printf("%s", buffer);

    bzero(buffer,1024);
    scanf("%s", buffer);
    send(sock,buffer,strlen(buffer),0);

    bzero(buffer,1024);
    // scanf("%s", buffer);
    // send(sock,buffer,strlen(buffer),0);

    // bzero(buffer,1024);
    read(sock,buffer,1024);
    printf("%s", buffer);
    bzero(buffer,1024);
            
}

int main(void) {
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    char cmd[10] ;
    printf("main menu\n 1.register\n 2.login\n");
    while (1) {
        pthread_t thread;
        char username[99];
        char password[99];
        printf("input:");
        scanf("%s", buffer);
        send(sock,buffer,strlen(buffer),0);

        if(!strcmp(buffer, "1")){
            pthread_create(&(thread), NULL, &regis, NULL);
        }

        else if(!strcmp(buffer, "2")){
            pthread_create(&(thread), NULL, &login, NULL);
        }

        else if(!strcmp(buffer, "0")){
            pthread_create(&(thread), NULL, &add, NULL);
        }
        pthread_join(thread,NULL);
		
    }
}