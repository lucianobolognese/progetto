/*
 ============================================================================
 Name        : clienttcp.c
 Author      : Luciano Bolognese
 Version     :
 Copyright   : Your copyright notice
 Description : Client TCP of a calculator that takes numbers in this order:
 	 	 	   [OPERATION] [FIRST NUMBER] [SECOND NUMBER]
 	 	 	   The connection with a server stop when the client send a "="
 ============================================================================
 */

#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif

#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"
#include <time.h>


void error(char *errorMessage) {
	printf("%s", errorMessage);
}

void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

int main(int argc, char *argv[]) {
#if defined WIN32
// Initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif

	char address [15];
	int port;

//Taking address as parameter .\clienttcp address port

	if (argv[1]>0 && argv[2]>0) {
		strcpy(address,argv[1]);
		port=atoi(argv[2]);
	}
	else {
		strcpy(address,ADDR);
		port=PROTOPORT;
	}

	int c_socket;
	c_socket= socket(AF_INET, SOCK_STREAM, 0);

	if (c_socket < 0) {
		error("Socket's creation failed.");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}

//Building the server's address
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	//int port=PROTOPORT;
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP del server
	sad.sin_port = port; // Server port

//Server connection

	int status = connect(c_socket, (struct sockaddr *)&sad, sizeof(sad));
	char  server_response[512];

	if (status == -1)
	{
	error( "Failed to connect.\n" );
	closesocket(c_socket);
	clearwinsock();
	system("pause");
	return 0;
	}
	else {

	printf("Connection established with %s : %d\n", address, port);
	memset(&server_response, 0, BUFFERSIZE);
	recv(c_socket, server_response, BUFFERSIZE,0);
	}

//Print out server response
	printf("Server:%s\n", server_response);

	char input_string1 [512]; //first standard input string
	char input_string2 [512]; //second standard input string
	char input_string3 [512];
	char server_string [512]; //server response
	char exit [512]="=";
	char var [512];

	while(1){
		memset(&input_string1, 0, sizeof(input_string1));
		memset(&input_string1, 0, sizeof(input_string2));
		memset(&input_string1, 0, sizeof(input_string3));
		printf("Insert an operation (ex. + 31 12 or '=' to close the calculator): ");
		scanf("%s",input_string1);

		strcpy(var,input_string1);
		if(strcmp(var,exit)==0 ){
			send(c_socket, input_string1, BUFFERSIZE, 0);
			closesocket(c_socket);
			printf("[+]Client disconnected.\n\n");
			system("pause");
			return 0;
		}
		scanf("%s",input_string2);
		scanf("%s",input_string3);


	    send(c_socket, input_string1, BUFFERSIZE, 0);
	    send(c_socket, input_string2, BUFFERSIZE, 0);
	    send(c_socket, input_string3, BUFFERSIZE, 0);
	    memset(&server_string, 0, sizeof(server_string));
	    recv(c_socket,server_string,BUFFERSIZE,0);
	 	printf("Server:%s\n", server_string);


	}

//Closing of connection
	closesocket(c_socket);
	clearwinsock();
	printf("Disconnesso\n"); // Print a final linefeed
	system("pause");
	return(0);
}


