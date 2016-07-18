#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main (int argc, void *argv[])
{
	int sock;
	socklen_t  clilen; 			
	char line[255]; 		// Массив для отправляемой строки
	struct sockaddr_in cliaddr, servaddr; // Структуры для адресов сервера и клиента 
	sock = socket(AF_INET, SOCK_DGRAM, 0);	//создание UDP-сокета
	if(sock < 0)
    {
        perror("socket");
        exit(-1);
    }
    //заполнение структуры
    servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(2299);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clilen = sizeof(servaddr);
	while (1){ 
		// Считываем и отправляем
		printf("Сообщение: ");
		scanf("%s", line);
		if (!strcmp(line, "exit"))
			break;
		if (sendto(sock, line, 256, 0, (struct sockaddr *)&servaddr, clilen) == -1)
			perror("sendto");
		//Принимаем
		if (recvfrom(sock, line, 256, 0, (struct sockaddr *)&servaddr, &clilen) == -1)
			break;
		printf("Вернул: %s\n", line);
	}

	// Завершение
	close(sock);
	exit(0);
}

