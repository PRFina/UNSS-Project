/*
 * config.c
 *
 *  Created on: 29 ott 2016
 *      Author: Pio Raffaele Fina
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "ErrorHandling.h"
#include "LogSystem.h"
#include "srvLimits.h"

void readConfigFile(const char* filePath, struct SRVCONFIG* srvSetup)
{
	/*
	 * Read configuration file, if SRVCONFIG structure has undergone changes,
	 * this function should be modified accordingly.
	 * All the trapped error will be managed has fatal error(show msg and kill the process),
	 * because the conf. file sets fundamental parameters!
	 */
	FILE *myFile;

	char buff[MAX_BUFF_SIZE];
	char key[MAX_SHRT_STR_LEN];
	char value[MAX_MID_STR_LEN];
	char* endptr;


	myFile=fopen(filePath,"r");
	if(myFile != NULL)
	{
		// Read line by line and save in the struct
		while( fgets(buff,MAX_BUFF_SIZE,myFile) != NULL)
		{
			//Parse line( key = value)
			sscanf(buff,"%s %*s %s",key,value); //TODO Make more robust regex to read key-value, because if sign = isn't separate from key string, there'll be a bug
			// DEBUG printf("Key: %s & value: %s\n",key,value);

			// Set SRVCONFIG field values
			if( strcmp(key,"srv_addr") == 0)
				strcpy(srvSetup->serv_addr, value);

			else if( strcmp(key,"srv_port") == 0)
				srvSetup->serv_port= strtol(value,&endptr,10); //convert from string to numeric value

			else if( strcmp(key,"max_queue_len") == 0)
				srvSetup->max_queue_len= strtol(value,&endptr,10); //convert from string to numeric value

			else if( strcmp(key,"log_file") == 0)
			{
				if( strcmp(value,"true") == 0 || strcmp(value,"True") == 0)
					srvSetup->log_file= 1;
				else if( strcmp(value,"false") == 0 || strcmp(value,"False") == 0)
					srvSetup->log_file= 0;
				else
					ErrorHandlerNoSock("011","Wrong configuration file syntax");
			}
			else
				ErrorHandlerNoSock("011","Wrong configuration file syntax");
		}
		fclose(myFile);
	}
	else
		ErrorHandlerNoSock("010","No configuration file available");

	//DEBUG printf("addr:%s\n port:%d\n queue:%d\n log:%d\n ",srvSetup->serv_addr,srvSetup->serv_port,
			//srvSetup->max_queue_len,srvSetup->log_file);
}

int initServer(const struct SRVCONFIG srvSetup, int* welcomeSocket, struct sockaddr_in* srvAddr)
{
	//create welcome socket(a.k.a server socket)
	*welcomeSocket= socket(PF_INET,SOCK_STREAM,0);
	if (*welcomeSocket < 0)
		printf("Socket() error");

	initLog(srvSetup.log_file);

	//init address(set number port e ip address)
	memset(srvAddr,0,sizeof(*srvAddr));
	srvAddr->sin_family=AF_INET;
	srvAddr->sin_addr.s_addr= inet_addr(srvSetup.serv_addr); //convert ip address
	srvAddr->sin_port=htons(srvSetup.serv_port);

	// Bind process,addr,port
	if( bind(*welcomeSocket, (struct sockaddr*) srvAddr, sizeof(*srvAddr)) < 0)
			ErrorHandler("02X","bind() problem",*welcomeSocket);

	// Server listen to connections
	if(listen(*welcomeSocket,srvSetup.max_queue_len) < 0)
	{
		logInfoFile(srvAddr,"Connection",0);
		ErrorHandler("1XX","listen() problem",*welcomeSocket);
	}

	return 0;
}
