/*
 ============================================================================
 Name        : servertcp.c
 Author      : Luciano Bolognese
 Version     :
 Copyright   : Your copyright notice
 Description : Server TCP of a calculator that takes numbers in this order:
 	 	 	   [OPERATION] [FIRST NUMBER] [SECOND NUMBER]
 	 	 	   The connection with a client stop when the client send a "="
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
#include <ctype.h>
#include <string.h>





void error(char *errorMessage) {
	printf("%s", errorMessage);
}

int add(int a,int b) {
	int result;
	result=a+b;
	return result;
}

int sub(int a,int b) {
	int result;
	result=a-b;
	return result;
}

int mult(int a,int b) {
	int result;
	result=a*b;
	return result;
}

int division(int a,int b) {
	float result;
	result=a/b;
	return result;
}

void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

int chat(int c_socket,int s_socket){
	int a;
	int b;
	char input_string1 [512];
	char input_string2 [512];
	char input_string3 [512];
	char var [512];
	char server_message[512]="Connection established\n";
	memset(&input_string1, 0, sizeof(input_string1));
	memset(&input_string2, 0, sizeof(input_string2));

	while(1){


	recv(c_socket,input_string1,BUFFERSIZE,0);
	recv(c_socket,input_string2,BUFFERSIZE,0);
	recv(c_socket,input_string3,BUFFERSIZE,0);

	printf("Client:%s ", input_string1);
	printf("%s", input_string2);
	printf(" %s\n", input_string3);


	memset(&server_message, 0, sizeof(server_message));
	strcpy(var, input_string1);

	a=atoi(input_string2);
	b=atoi(input_string3);
	int c;
	char op=input_string1[0];
	char result [512];

	switch(op)
	{
	case '+':
		c=add(a,b);
		itoa(c,result,10);
		strcpy(server_message, result);
		break;
	case '/':
		if (b!=0){
			c=division(a,b);
			itoa(c,result,10);
			strcpy(server_message, result);
		}
		else {
			strcpy(server_message, "Math error");
		}
		break;
	case '*':
		c=mult(a,b);
		itoa(c,result,10);
		strcpy(server_message, result);
		break;
	case '-':
		c=sub(a,b);
		itoa(c,result,10);
		strcpy(server_message, result);
		break;

	case '=':
		break;

	default:
		strcpy(server_message, "Wrong operation");
		break;
	}

	send(c_socket, server_message, strlen(server_message), 0);

	if(op=='='){
		printf("[+]Client disconnected.\n\n");
		return 0;
	}
	}
}


int main(int argc, char *argv[]) {

	char server_message[512]="Connection established\n";
	int port=PROTOPORT;
	if (argc > 1) {
		port = atoi(argv[1]); //convert the specified argument to binary
	}
	else
		port = PROTOPORT;
	if (port < 0) {
		printf("Wrong port number %s \n", argv[1]);
		return 0;
	}
#if defined WIN32 // Winsock initialization
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);


	if (result != NO_ERROR) { //error checking in winsock initialization
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif

//socket's creation

	int s_socket;
	s_socket = socket(AF_INET, SOCK_STREAM, 0);

//check error in socket's initialization

	if (s_socket < 0) {
		error ("socket creation failed. \n");
		return -1;
	}


//Assignment of an address to the socket bind()

	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); //check that bit extra contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	sad.sin_port = port; //convert order of byte within comunication through host and network
	int n;

	n=bind(s_socket, (struct sockaddr*) &sad, sizeof(sad));
	if (n < 0)  {
		error ("bind() failed. \n");
		closesocket(s_socket);
		clearwinsock();
		return -1;
	}


//Setting socket to listen status
	while(1){

		if (listen (s_socket, QLEN) < 0) {
			error ("listen() failed.\n");
			closesocket(s_socket);
			clearwinsock();
			return -1;
		}

		struct sockaddr_in cad; // structure for the client address
		int c_socket; // socket descriptor for the client
		int client_len; // the size of the client address
		printf("Waiting for a client to connect...\n");

//Iterate the life of server socket
			c_socket = accept(s_socket, (struct sockaddr*)&cad, &client_len);
			printf("[+]Client connected.\n");

			memset(&server_message, 0, sizeof(server_message));
			strcpy(server_message, "Connection established");
			printf("Server: %s %i\n", server_message,port);
			send(c_socket, server_message, strlen(server_message), 0);

//Condition of chatting Client-Server
			if(chat(c_socket,s_socket)==0){
				continue;
			}
	} //while end
}//main end


