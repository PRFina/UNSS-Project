/*
 * ServerMain.c
 *
 *  Created on: 27 ott 2016
 *      Author: Pio Raffaele Fina
 */
//

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
#include "Utils.h"
#include "config.h"
#include "LogSystem.h"


/* Naming convention
 * srv: server
 * addr: address
 * clt: client
 * msg: message.
 */



int main(int argc, char *argv[]) {


	// Server variables
	int welcomeSocket;
	struct sockaddr_in welcomeSockAddr;
	struct SRVCONFIG srvSetup;

	// Client address struct info
	struct sockaddr_in cSock;

	int clientSocket;
#if defined WIN32
	int clientLen;
#endif
#if defined linux
	socklen_t clientLen;
#endif



#if defined WIN32
	//Init WINSOCK library
	WSADATA wsaData;
	int iResult;
	iResult=WSAStartup(MAKEWORD(2,2),&wsaData);
	if(iResult!= 0)
		showInfoMessage("Internal System Error: winSock library failed to initialize");
#endif
	/*****************SERVER CODE START HERE****************/

	//Read config file and setup server structure
	showInfoMessage("Reading configuration file...");
	readConfigFile("..//Release//CONFIG.txt", &srvSetup);


	//Init Server
	showInfoMessage("Initializing server...");
	initServer(srvSetup, &welcomeSocket, &welcomeSockAddr);

	// Server's core
	memset(&cSock,0,sizeof(cSock));
	clientLen=sizeof(cSock);
	logInfo(&welcomeSockAddr,"Server Start",0,LOG_BOTH);
	while(1)
	{

		printf("Wating for connection from...\n\n");

		clientSocket=accept(welcomeSocket, (struct sockaddr*) &cSock, &clientLen); // Accept new client socket
		if(clientSocket < 0)
			ErrorHandler("1XX","Accept() problem",welcomeSocket);

		logInfo(&cSock,"Open Connection",0,LOG_BOTH);
		handleClient(clientSocket);
		logInfo(&cSock,"Close Connection",0,LOG_BOTH);

		closesocket(clientSocket);

	}
#if defined WIN32
	WSACleanup();
#endif
	systemPause(); //Portable system("PAUSE");

	return 0;
}
