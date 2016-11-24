/*
 * ErrorHandling.h
 *
 *  Created on: 28 ott 2016
 *      Author: Pio Raffaele Fina
 */

#ifndef ERRORHANDLING_H_
#define ERRORHANDLING_H_

void ErrorCodeMsg(const char* errCode, const char* addInfo);
void ErrorHandler(const char* errCode, const char* addInfo,  const int socketDescriptor);
void ErrorHandlerNoSock(const char* errCode, const char* addInfo);
void showInfoMessage(const char* msg);


#endif /* ERRORHANDLING_H_ */
