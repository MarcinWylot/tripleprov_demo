/*
 * Server.h
 *
 *  Created on: 02-04-2013
 *      Author: marcin
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "diplodocus.h"
#include <arpa/inet.h>          /* sockaddr_in, inetd_addr() */
#include <netinet/in.h>         /* sockaddr_in{} and other Internet defns */
#include <sys/socket.h>         /* socket(), connect() */
#include <arpa/inet.h>          /* sockaddr_in, inetd_addr() */
#include <netinet/in.h>         /* sockaddr_in{} and other Internet defns */
#include <netdb.h>          //gethostbyname(), getservbyname()
#include <stdlib.h>         //atoi()
#include <unistd.h>         //close()
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <linux/tcp.h>


class NetworkNode {
//	tcp::iostream* f2;

public:
//	vector<KEY_ID> molecules;
//	fstream *f;
//	NetworkNode();
	int fd;
	NetworkNode(int _fd);
	void Send(const char *buf, size_t size);
	void Receive(char *buf, size_t size);
	virtual ~NetworkNode();
};
class Server {
	vector<NetworkNode> nodes;
public:
	void Start();
	void Send(const char *buf, size_t size, size_t node);
	void Receive(char *buf, size_t size, size_t node);
	void ReceiveFromAllSelect(vector<KEY_ID> *results); //FIXME results cannot be like this, should be more generic
	size_t GetNbOfNodes();
	Server();
	virtual ~Server();
};

namespace diplo {
	extern Server SRV;
}

#endif /* SERVER_H_ */
