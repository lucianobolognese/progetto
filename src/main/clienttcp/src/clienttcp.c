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

	//creazione della socket

	int c_socket;
	c_socket= socket(AF_INET, SOCK_STREAM, 0);

	if (c_socket < 0) {
		error("creazione della socket fallita.");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}
	else printf("socket creata\n");

	// COSTRUZIONE DELL’INDIRIZZO DEL SERVER
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	int port=PROTOPORT;
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr( "127.0.0.1" ); // IP del server
	sad.sin_port = htons(port); // Server port

	// CONNESSIONE AL SERVER

/*	int status = */connect(c_socket, (struct sockaddr *)&sad, sizeof(sad));
/*
	if (status == -1)
	{
	error( "Failed to connect.\n" );
	closesocket(c_socket);
	clearwinsock();
	}
*/
	printf("connessione stabilita alla porta %d\n", port);
	char  server_response[512];
	recv(c_socket, server_response, sizeof(server_response),0);

	// print out server response
	printf("Il server ha mandato: %s \n", server_response);
	printf("dioboia");

	//close the socket
	closesocket(c_socket);
	printf("Disconnesso dal server.\n");
	return 0;
}

/*
	char* input_string = "prova"; // Stringa da inviare
	int string_len = strlen(input_string); // Determina la lunghezza

	// INVIARE DATI AL SERVER
	if (send(c_socket, input_string, string_len, 0) != string_len)
	{
	error("send() sent a different number of bytes than expected");
	closesocket(c_socket);
	clearwinsock();
	return -1;
	}

	// RICEVERE DATI DAL SERVER
	int bytes_rcvd;
	int total_bytes_rcvd = 0;
	int n;
	char buf[BUFFERSIZE]; // buffer for data from the server
	printf("Received: "); // Setup to print the echoed string
	while (total_bytes_rcvd < string_len) {

	if ((bytes_rcvd = recv(c_socket, buf, BUFFERSIZE - 1, 0)) <= 0)
	{
	error("recv() failed or connection closed prematurely");
	closesocket(c_socket);
	clearwinsock();
	return -1;
	}
	total_bytes_rcvd += bytes_rcvd; // Keep tally of total bytes
	buf[bytes_rcvd] = '\0'; // Add \0 so printf knows where to stop
	n=read(c_socket, buf, BUFFERSIZE);
	printf("%s", buf); // Print the echo buffer
	}
	// CHIUSURA DELLA CONNESSIONE
	closesocket(c_socket);
	clearwinsock();
	printf("\n"); // Print a final linefeed
	system("pause");
	return(0);
	}
*/
