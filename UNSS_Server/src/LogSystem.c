/*
 * LogSystem.c
 *
 *  Created on: 30 ott 2016
 *      Author: Pio Raffaele Fina
 */

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

#include "LogSystem.h"
#include "Utils.h"
#include "srvLimits.h"

//FUTURE find a way to read dfilepath directly from srvr config file
#define LOG_FILE_PATH "..//Release//LogInfo.txt"

static int isLogActive=0;

void initLog(int enableLogSystem)
{
	/*
	 * Implemented a mechanism to read from config file if log system is enabled,
	 * and set the global(only in this module) variable to 1 for true.
	 */
	if(enableLogSystem == 1)
		isLogActive=1;
}
void logInfoFile(const struct sockaddr_in* clientInfo, const char* msg, int errType)
{
	/*
	 * Syntax:
	 * <timestamp>+<log cateogry>+<client host name>+<client ip>+<message>
	 */
	FILE* logFile;
	char hostName[MAX_MID_STR_LEN];
	char errTypeMsg[MAX_SHRT_STR_LEN];
	// Get client name TODO getHostname return local machine name, change with getnameinfo() function.
	gethostname(hostName,sizeof(hostName));

	//Check Type error
	if(errType == 0)
		strcpy(errTypeMsg,"INFO");
	else if(errType == 1)
		strcpy(errTypeMsg,"ERROR");

	//Check if log system is active and write info to the log file
	if( isLogActive == 1)
	{
		if((logFile=fopen(LOG_FILE_PATH,"a")) != NULL)
			{
				fprintf(logFile,"%s %s %s %s %s \n", getTimeStamp(),errTypeMsg, hostName, inet_ntoa(clientInfo->sin_addr), msg);
				fclose(logFile);
			}
		else
			printf("ERROR IN WRITING LOG FILE!!!!!!");
	}

}

void logInfoTerminal(const struct sockaddr_in* clientInfo, const char* msg, int errType)
{
	/*
	 * Syntax:
	 * <timestamp>+<log cateogry>+<client host name>+<client ip>+<message>
	 */
	char hostName[MAX_MID_STR_LEN];
	char errTypeMsg[MAX_SHRT_STR_LEN];
	/*
	 * Get client name TODO getHostname return local machine name, change with getnameinfo() function.
	 * seems that getnameinfo()isn't defined in any header files, how to solve this?
	 */
	gethostname(hostName,sizeof(hostName));

	//Check Type error
	if(errType == 0)
		strcpy(errTypeMsg,"INFO");
	else if(errType == 1)
		strcpy(errTypeMsg,"ERROR");

	//Check if log system is active and write info to the log file
	if( isLogActive == 1)
	{
		fprintf(stdout,"%s %s %s %s %s \n", getTimeStamp(),errTypeMsg, hostName, inet_ntoa(clientInfo->sin_addr), msg);

	}

}

void logInfo(const struct sockaddr_in* clientInfo, const char* msg, int errType, int logFlag)
{
	/*
	 * Wrap function, call logFile,logTerminal or both
	 */
	if( logFlag == LOG_FILE)
		logInfoFile(clientInfo,msg,errType);
	else if( logFlag == LOG_TERMINAL)
		logInfoTerminal(clientInfo,msg,errType);
	else if( logFlag == LOG_BOTH)
	{
		logInfoFile(clientInfo,msg,errType);
		logInfoTerminal(clientInfo,msg,errType);
	}
}
