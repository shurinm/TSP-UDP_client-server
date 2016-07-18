#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int sockfd; 			// Дескриптор сокета
    int clilen, n; 			// Переменные для различных длин и количества символов 
    char line[256]; 		// Массив для принятой и отсылаемой строки 
    struct sockaddr_in servaddr, cliaddr; // Структуры для адресов сервера и клиента 
    //bzero(&servaddr, sizeof(servaddr));
    // Создаем UDP сокет 
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket"); 
        exit(1);  
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(2299);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Настраиваем адрес сокета 
    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror("bind");
        close(sockfd);
        exit(1);
    }

    clilen = sizeof(cliaddr); 
    while(1) {
        // принимаем текст
        if((n = recvfrom(sockfd, line, 256, 0,(struct sockaddr *) &cliaddr, &clilen)) < 0){
            perror("recvfrom");
            close(sockfd);
            exit(-1);
        }
        // Печатаем принятый текст на экране 
        printf("%s\n", line);
        // Принятый текст отправляем обратно по адресу отправителя 
        if(sendto(sockfd, line, strlen(line), 0,(struct sockaddr *) &cliaddr, clilen) < 0){
            perror("sendto");
            close(sockfd);
            exit(-1);
        }
    }

    return 0;
}