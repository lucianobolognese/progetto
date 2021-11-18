/*
 ============================================================================
 Name        : servertcp.c
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

int main(int argc, char *argv[]) {

	char server_message[512]="Connessione avvenuta\n";
	int port=PROTOPORT;
	if (argc > 1) {
		port = atoi(argv[1]); //converte l'argomento specificato in binario
	}
	else
		port = PROTOPORT;
	if (port < 0) {
		printf("Numero di porta sbagliato %s \n", argv[1]);
		return 0;
	}
#if defined WIN32 // Inizializza Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);


	if (result != NO_ERROR) { //controllo errori
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif

	//creazione della socket

	int s_socket;
	s_socket = socket(AF_INET, SOCK_STREAM, 0);

	//controllo errori inizializzazione socket

	if (s_socket < 0) {
		error ("socket creation failed. \n");
		return -1;
	}


	//assegnazione di un indirizzo alla socket bind()

	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); //controlla che i bit extra contengono 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	sad.sin_port = port; //converte l'ordine dei byte nella comunicazione fra host e network
	int n;

	n=bind(s_socket, (struct sockaddr*) &sad, sizeof(sad));
	if (n < 0)  {
		error ("bind() failed. \n");
		closesocket(s_socket);
		clearwinsock();
		return -1;
	}


// SETTAGGIO DELLA SOCKET ALL'ASCOLTO listen()

	if (listen (s_socket, QLEN) < 0) {
		error ("listen() failed.\n");
		closesocket(s_socket);
		clearwinsock();
		return -1;
	};





// ACCETTARE UNA NUOVA CONNESSIONE
	for(;;){

		struct sockaddr_in cad; // structure for the client address
		int c_socket; // socket descriptor for the client
		int client_len; // the size of the client address
		printf("Aspettando che un Client si connetta...\n");


		while(1){
			c_socket = accept(s_socket, (struct sockaddr*)&cad, &client_len);
			char input_string1 [512];
			char input_string2 [512];
			char input_string3 [512];
			int a;
			int b;

			printf("[+]Client connected.\n");
			int status1;
			int status2;
			int status3;
			char var [512];
			char exit [512]="exit";


			memset(&server_message, 0, sizeof(server_message));
			strcpy(server_message, "Connection established");
			printf("Server: %s %i\n", server_message,port);
			send(c_socket, server_message, strlen(server_message), 0);


				while(1){
					memset(&input_string1, 0, sizeof(input_string1));
					memset(&input_string2, 0, sizeof(input_string2));

					status1=recv(c_socket,input_string1,BUFFERSIZE,0);
					status2=recv(c_socket,input_string2,BUFFERSIZE,0);
					status3=recv(c_socket,input_string3,BUFFERSIZE,0);
					printf("Client:%s ", &input_string1);
					printf("%s", &input_string2);
					printf(" %s\n", &input_string3);


					memset(&server_message, 0, sizeof(server_message));
					strcpy(var, &input_string1);

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
	    		sleep(2);
	    	}
	    }

				printf("asdad");
				system("pause");
				return 0;
	 }

}
}

/*

	// CHIUSURA DELLA CONNESSIONE
	printf("Handling client %s\n", inet_ntoa(cad.sin_addr));
	} // end-while

	closesocket(s_socket);
	clearwinsock();
	return 1;
} // main end

*/
