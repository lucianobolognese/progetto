/*
 * protocol.h server TCP
 *
 *  Created on: 4 nov 2021
 *      Author: Luciano Bolognese
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define BUFFERSIZE 512
#define PROTOPORT 27015
#define ADDR "127.0.0.1"
#define QLEN 5
#define NO_ERROR 0


struct {
	int a;
	int b;
} msgStruct;



#endif /* PROTOCOL_H_ */
