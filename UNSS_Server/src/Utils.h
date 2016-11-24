/*
 * Utils.h
 *
 *  Created on: 28 ott 2016
 *      Author: Pio Raffaele Fina
 */

#ifndef UTILS_H_
#define UTILS_H_




char* getTimeStamp();

void handleClient(int socket);

char* str_toUpper(const char* strSource);

void systemPause();
#endif /* UTILS_H_ */
