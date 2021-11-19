/*
 ============================================================================
 Name        : clienttcp.c
 Author      : luciano
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
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
	/*

	char address [15];
	printf("Insert an address:");
	gets(address);
	if (strcmp(address,ADDR)!=0){
		error("Invalid address");
	}

	//creazione della socket

	int port;
	char string_port [5];
	printf("Insert a port: ");
	gets(string_port);
	port=atoi(string_port);
*/

	int c_socket;
	c_socket= socket(AF_INET, SOCK_STREAM, 0);

	if (c_socket < 0) {
		error("creazione della socket fallita.");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}

	// COSTRUZIONE DELL’INDIRIZZO DEL SERVER
	struct sockaddr_in sad;
	int s_socket;
	memset(&sad, 0, sizeof(sad));
	//int port=PROTOPORT;
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP del server
	sad.sin_port = port; // Server port

	// CONNESSIONE AL SERVER

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

	// print out server response
	printf("Server:%s\n", server_response);

	char input_string1 [512]; //prima stringa std input
	char input_string2 [512]; //seconda stringa std input
	char input_string3 [512];
	char server_string [512]; //risposta server
	char exit [512]="=";
	char var [512];
    int status1;

	while(1){
		memset(&input_string1, 0, sizeof(input_string1));
		memset(&input_string1, 0, sizeof(input_string2));
		memset(&input_string1, 0, sizeof(input_string3));
		printf("Inserisci una stringa: ");
		scanf("%s",input_string1);

		strcpy(var,input_string1);
		if(strcmp(var,exit)==0 ){
			send(c_socket, input_string1, BUFFERSIZE, 0);
			//sleep(0,2);
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
	    status1=recv(c_socket,server_string,BUFFERSIZE,0);
	 	printf("Server:%s\n", &server_string);


	}
	/*
	// INVIARE DATI AL SERVER
	if (send(c_socket, input_string1, sizeof(input_string1), 0) != sizeof(input_string1))
	{
	error("send() sent a different number of bytes than expected");
	closesocket(c_socket);
	clearwinsock();
	return -1;
	}

	if (send(c_socket, input_string2, sizeof(input_string2), 0) != sizeof(input_string2))
		{
		error("send() sent a different number of bytes than expected");
		closesocket(c_socket);
		clearwinsock();
		return -1;
		}
*/

	// CHIUSURA DELLA CONNESSIONE
	closesocket(c_socket);
	clearwinsock();
	printf("Disconnesso\n"); // Print a final linefeed
	system("pause");
	return(0);
}
	//close the socket

