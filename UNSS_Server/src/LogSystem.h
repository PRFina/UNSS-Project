/*
 * LogSystem.h
 *
 *  Created on: 30 ott 2016
 *      Author: Pio Raffaele Fina
 */

#ifndef LOGSYSTEM_H_
#define LOGSYSTEM_H_


/*
 *
 */
#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

#include "config.h"

#define LOG_FILE 0
#define LOG_TERMINAL 1
#define LOG_BOTH 2


void initLog(int enableLogSystem);
void logInfo(const struct sockaddr_in* clientInfo, const char* msg, int errType, int logFlag);
void logInfoFile(const struct sockaddr_in* clientInfo, const char* msg, int errType);
void logInfoTerminal(const struct sockaddr_in* clientInfo, const char* msg, int errType);

#endif /* LOGSYSTEM_H_ */
