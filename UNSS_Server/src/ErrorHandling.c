/*
 * ErrorHandling.c
 *
 *  Created on: 28 ott 2016
 *      Author: Pio Raffaele Fina
 */

#include <stdio.h>
#include <stdlib.h>
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
#include "Utils.h"


void ErrorCodeMsg(const char* errCode, const char* addInfo)
{
	/*
	 * Check the errorCode category and print the message in the syntax form:
	 * <category error>+ <additional info>+ <error code>
	 * New error messages should be managed here
	 */
	if( errCode[0] == '0')
		{
			printf("Server init error! %s Error code %s \n", addInfo, errCode);
		}
		else if( errCode[0] == '1')
		{
			printf("Connection error! %s Error code %s \n", addInfo, errCode);
		}
		else if( errCode[0] == '2')
		{
			printf("Transfer error! %s Error code %s \n", addInfo, errCode);
		}
}
void ErrorHandler(const char* errCode, const char* addInfo, const int socketDescriptor)
{
	//Print message
	ErrorCodeMsg(errCode,addInfo);

	//Close associated resources and close the process.
	closesocket(socketDescriptor);
#if defined WIN32
	WSACleanup();
#endif
	printf("Exiting the process..\n");
	systemPause(); //Portable system("PAUSE");
	exit(EXIT_FAILURE);
}

void ErrorHandlerNoSock(const char* errCode, const char* addInfo)
{

	ErrorCodeMsg(errCode,addInfo);
	//Close associated resources and close the process
#if defined WIN32
	WSACleanup();
#endif
	printf("Exiting the process..\n");
	systemPause(); //Portable system("PAUSE");
	exit(EXIT_FAILURE);
}

void showInfoMessage(const char* msg)
{
	/*
	 * Show info message on server console, nothing fancy.
	 * just display single msg per line adding a new line at the end.
	 */
	printf("%s\n",msg);
}
