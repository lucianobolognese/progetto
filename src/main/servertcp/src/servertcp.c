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





void error(char *errorMessage) {
	printf("%s", errorMessage);
}



void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

int main(int argc, char *argv[]) {

	char server_message[512]="Connessione avvenuta";

	int port;
	if (argc > 1) {
		port = atoi(argv[1]); //converte l'argomento specificato in binario
	}
	else
		port = htons(27015); //usa il port number impostato nell'header di default
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
	sad.sin_port = htons(port); //converte l'ordine dei byte nella comunicazione fra host e network
	int n;

	n=bind(s_socket, (struct sockaddr*) &sad, sizeof(sad));
	if (n < 0)  {
		error ("bind() failed. \n");
		closesocket(s_socket);
		clearwinsock();
		return -1;
	}
	printf("bind corretto alla porta %d\n", ntohs(port));


// SETTAGGIO DELLA SOCKET ALL'ASCOLTO listen()
	listen(s_socket,5);


	/*if (listen (s_socket, QLEN) < 0) {
	error ("listen() failed.\n");
	closesocket(s_socket);
	clearwinsock();
	return -1;
	}
	else printf("Server in ascolto\n");*/





// ACCETTARE UNA NUOVA CONNESSIONE

	struct sockaddr_in cad; // structure for the client address
	int c_socket; // socket descriptor for the client
	int client_len; // the size of the client address
	printf("Aspettando che un Client si connetta...\n");

	//c_socket = accept(s_socket, (struct sockaddr*)&cad, &client_len);
/*
	while (1) { // oppure for (;;)
	if ((c_socket = accept(s_socket, (struct sockaddr*)&cad, &client_len)) < 0) {
	error("accept() failed.\n");
	}
	else printf("connessione stabilita.");

	//send the message
	send(c_socket, server_message, strlen(server_message),0);
	//close socket
	closesocket(s_socket);

	return 0;
}
*/
	 while(1){
	    c_socket = accept(s_socket, (struct sockaddr*)&cad, &client_len);
	    printf("[+]Client connected.\n");


	    memset(&server_message, 0, sizeof(server_message));
	    strcpy(server_message, "Connessione avvenuta");
	    printf("Server: %s\n", server_message);
	    send(c_socket, server_message, strlen(server_message), 0);

	    closesocket(c_socket);
	    printf("[+]Client disconnected.\n\n");
	 }
	return 0;
}


/*
	// MANDARE DATI AL CLIENT
	char* input_string="Connessione avvenuta";
	int string_len =sizeof(input_string);

	if (send(s_socket, input_string, string_len, 0) != string_len)
		{
		error("send() sent a different number of bytes than expected");
		closesocket(s_socket);
		clearwinsock();
		return -1;
		}



	// CHIUSURA DELLA CONNESSIONE
	printf("Handling client %s\n", inet_ntoa(cad.sin_addr));
	} // end-while

	closesocket(s_socket);
	clearwinsock();
	return 1;
} // main end

*/
