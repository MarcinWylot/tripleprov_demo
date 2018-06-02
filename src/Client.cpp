/*
 * Client.cpp
 *
 *  Created on: 02-04-2013
 *      Author: marcin
 */

#include "Client.h"

Client::Client() {
fd = -1;
}

void Client::Send(char* buf, size_t size) {
	size_t len = 0;
	while (len < size) {
		size_t n = write(fd, (buf + len), size - len);
		if (n <= 0) {
			cerr << "NetworkNode::Send: " << endl;
			ERROR_OUT
		}
		len += n;

	}
}

void Client::Receive(char* buf, size_t size) {
	size_t len = 0;
	while (len < size) {
		size_t n = read(fd, (buf + len), size - len);
		if (n <= 0) {
			cerr << "NetworkNode::Receive: " << n << endl;
			ERROR_OUT
//			perror("NetworkNode::Receive: ");
			exit(0);
		}
		len += n;

	}
}

void Client::Start() {
	int sockfd, portno;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	portno = stoi(diplo::server_port);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int on = 1;
	setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &on, sizeof(on));

	on = 12582912;
	setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *) &on, sizeof(on));
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *) &on, sizeof(on));

	if (sockfd < 0)
		cerr << "ERROR opening socket";
	server = gethostbyname(diplo::server_adr.c_str());
	if (server == NULL) {
		cerr << "ERROR, no such host" << endl;
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr,
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
	serv_addr.sin_port = htons(portno);

	for (int i = 0; i <= 50; i++) {
		if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
			cerr << "connecting try no: "<< i << endl;
			sleep(10);
		} else {
			fd = sockfd;
			return;
		}
	}
	cerr << "ERROR connecting" << endl;
	exit(1);
}

Client::~Client() {
	// TODO Auto-generated destructor stub
}

