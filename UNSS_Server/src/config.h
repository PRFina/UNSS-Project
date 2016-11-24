/*
 * config.h
 *
 *  Created on: 29 ott 2016
 *      Author: Pio Raffaele Fina
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

#include "srvLimits.h"

struct SRVCONFIG {
	char serv_addr[MAX_MID_STR_LEN]; //Machine server IP-address.
	int serv_port; // Socket port.
	int max_queue_len; // max number of client in the server queue.
	int log_file; // bool: 1 to save log file, 0 not.
	char log_fpath[MAX_LNG_STR_LEN];
} ;

void readConfigFile(const char* filePath, struct SRVCONFIG* srvSetup);
int initServer(const struct SRVCONFIG srvSetup, int* welcomeSocket, struct sockaddr_in* srvAddr);


#endif /* CONFIG_H_ */
