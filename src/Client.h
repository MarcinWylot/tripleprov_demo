/*
 * Client.h
 *
 *  Created on: 02-04-2013
 *      Author: marcin
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "diplodocus.h"
#include "Server.h"

class Client {
//	tcp::iostream f;
	int fd;
public:
	void Send(char *buf, size_t size);
	void Receive(char *buf, size_t size);
	void Start();
	Client();
	virtual ~Client();
};

namespace diplo {
	extern Client CLI;
}

#endif /* CLIENT_H_ */
