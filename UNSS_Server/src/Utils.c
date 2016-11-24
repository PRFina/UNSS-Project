/*
 * Utils.c
 *
 *  Created on: 28 ott 2016
 *      Author: Pio Raffaele Fina
 */
#include <stdlib.h>
#include <stdio.h>
#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

#include <time.h>
#include <string.h>
#include <ctype.h>

#include "Utils.h"
#include "ErrorHandling.h"
#include "srvLimits.h"


char* getTimeStamp()
{
	time_t unixTime;
	struct tm *infoTime;
	char *timestamp;

	unixTime=time(NULL);
	infoTime=localtime(&unixTime); // convert from unixtime to tm_struct

	//Get a readable format iso 8601
	timestamp=malloc(sizeof(char)*MAX_MID_STR_LEN);
	strftime(timestamp,sizeof(char)*MAX_MID_STR_LEN,"%Y-%m-%dT%H:%M:%S",infoTime);

	return timestamp;//asctime(infoTime);
}


void handleClient(int cltSocket)
{
	/*
	 * Server business logic:
	 * Send, receive and process data
	 * max number allowed 2^32-1: long int/4 byte/32bit TODO provide error trap
	 */
	char welcomeMsg[]="Connected with server!!";
	int byteRecv=0;
	int numberMax=100;
	char cltMsg[MAX_SHRT_STR_LEN];

	//Send welcome msg to client
	if(send(cltSocket,welcomeMsg,strlen(welcomeMsg),0) != strlen(welcomeMsg))
		ErrorHandler("210","Error in sending data",cltSocket);

	int finish=1;
	while( finish == 1)
	{
		/* if client send wrong format data, server doesn't raise a fatal error because cant'send back to client anything.
		 * instead check if byte received < 0 and then exit from loop and close the connection.*/
		int data;
		if((byteRecv=recv(cltSocket,(char*)&data,sizeof(data),0)) < 0) // NOTE: casting to a char* due for function declaration parameter type
		{
			showInfoMessage("NO DATA RECEIVED");
			finish=0; // force exit
		}

			//ErrorHandler("201","No data received",cltSocket);
		else
		{
			data=ntohl(data); // convert in host format(little-endian x64 architecture)
			printf("Data Received:%d\n",data);

			if(data > numberMax)
			{
				strcpy(cltMsg,"QUIT"); // create QUIT "signal" to client
				finish=0; // Exit from loop
			}
			else
				strcpy(cltMsg,"CONTINUE"); // create CONTINUE "signal" to client


			if(send(cltSocket, cltMsg, strlen(cltMsg),0) != strlen(cltMsg)) //Send msg to client
				ErrorHandler("210","Error in sending data",cltSocket);
		}
	}
}

char* str_toUpper(const char* strSource)
{
	char* str=malloc(sizeof(char)*strlen(strSource)+1);
	int i=0;
	while( strSource[i] != '\0')
	{
		str[i]= toupper(strSource[i]);
		i++;
	}
	str[i]='\0';

	return str;
}

void systemPause()
{
	/*
	 * Since system("PAUSE") is platform dependent,
	 * this function emulate a pause in a portable way using getchar().
	 *
	 */

	printf("Press any key to continue . . .");
	getchar();
}
