/*
 ============================================================================
 Name        : UNSS_Client.c
 Author      : Fina Pio Raffale
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*
 * ClientMain.c
 *
 *  Created on: 27 ott 2016
 *      Author: Pio Raffaele Fina
 */

/* Naming convention
 * srv: server
 * addr: address
 * clt: client
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif
#include "ErrorHandling.h"
#include "cltLimits.h"

#define DEFAULT_SRV_ADDRESS "127.0.0.1"
#define DEFAULT_PORT 6790

int main(int argc, char *argv[]) {

	struct sockaddr_in serverAddr;
	int clientSocket;
#if defined WIN32
	//WinSock init
	WSADATA wsaData;
	int iResult;
	iResult=WSAStartup(MAKEWORD(2,2),&wsaData);
	if(iResult== 0)
		printf("WSA Client Success!\n");
	else
		printf("wsock error: %d",iResult);
#endif

	//Start client check
	if(argv[1] == NULL || argv[2] == NULL)
	{
		printf("WARNING: Not enough arguments, you must supply ip-address and port number, client will start with default settings!\n");
		//Default Server address info
		serverAddr.sin_family=AF_INET;
		serverAddr.sin_addr.s_addr=inet_addr(DEFAULT_SRV_ADDRESS);
		serverAddr.sin_port=htons(DEFAULT_PORT);
	}
	else
	{
		// Custom Server address info
		serverAddr.sin_family=AF_INET;
		serverAddr.sin_addr.s_addr=inet_addr(argv[1]);
		serverAddr.sin_port=htons(atoi(argv[2]));
	}


	//Client socket
	clientSocket= socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if( clientSocket < 0)
		ErrorHandler("0xx","client socket creation problem", clientSocket);



	//Connection
	if (connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0)
		ErrorHandler("100","Connection not established",clientSocket);

	//receive Welcome message from server
	char buffer[MAX_BUFF_SIZE];
	int recvByte;
	recvByte=recv(clientSocket,buffer,sizeof(buffer),0);
	buffer[recvByte]='\0';
	printf("Info: %s \n",buffer);

	int run=1;
	int data=0;
	int check=0;
	while(run == 1)
	{

		printf("Insert a number: ");


		//Get input
		if((check=scanf("%d",&data)) != 1 )
		{
			//TODO add a check for MAX limit number allowed
			/*
			 * NOTE: this "trap" doesn't check all the possible wrong numeric pattern (e.g. 12.45)
			 * for more robust way of handling stdin, we should create a specific parser with fgets() & sscanf(), or
			 * reading single char in a FSA way with getch(). Using fflush() with stdin isn't either an option
			 * because fflush() behaviour is defined only for some platform => not portable.
			 */
			ErrorHandler("210","Wrong input format!",clientSocket);
		}

		//Send to server
		data=htonl(data); //Convert endianess
		if(send(clientSocket,(char*)&data,sizeof(data),0) != sizeof(data))
			ErrorHandler("210","Error in sending data",clientSocket);

		//Receive from server
		recvByte=0;
		recvByte=recv(clientSocket, buffer, sizeof(buffer),0);
		buffer[recvByte]='\0'; // set end of the string
		printf("Server response: %s \n",buffer);

		if( strcmp(buffer,"QUIT") == 0) //Exit
			run=0;
	}

		closesocket(clientSocket);
	#if defined WIN32
		WSACleanup();
	#endif

	//system("PAUSE") emulation
	printf("Press any key to continue . . .");
	getchar();
	return 0;
}
