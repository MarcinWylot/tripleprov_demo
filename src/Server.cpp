/*
 * Server.cpp
 *
 *  Created on: 02-04-2013
 *      Author: marcin
 */

#include "Server.h"

/*
 *
 *
 echo 'net.core.wmem_max=12582912' >> /etc/sysctl.conf
 echo 'net.core.rmem_max=12582912' >> /etc/sysctl.conf
 echo 'net.ipv4.tcp_rmem= 10240 87380 12582912' >> /etc/sysctl.conf
 echo 'net.ipv4.tcp_wmem= 10240 87380 12582912' >> /etc/sysctl.conf
 echo 'net.ipv4.tcp_window_scaling = 1' >> /etc/sysctl.conf
 echo 'net.ipv4.tcp_sack = 1' >> /etc/sysctl.conf
 echo 'net.ipv4.tcp_no_metrics_save = 1' >> /etc/sysctl.conf
 *
 */

Server::Server() {

}

void Server::Send(const char* buf, size_t size, size_t node) {
	nodes[node].Send(buf, size);
}

void Server::Receive(char* buf, size_t size, size_t node) {
	nodes[node].Receive(buf, size);
}

size_t Server::GetNbOfNodes() {
	return nodes.size();
}

void Server::Start() {
	int sockfd, portno;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &on, sizeof(on));

	on = 12582912;
	setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *) &on, sizeof(on));
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *) &on, sizeof(on));

	if (sockfd < 0)
		cerr << "ERROR opening socket" << endl;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(diplo::server_port.c_str());
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		cerr << "ERROR on binding" << endl;
	listen(sockfd, 20);

	socklen_t addrlenl;
	struct sockaddr *addr = (struct sockaddr *) malloc(addrlenl);

	while (1) {
		cerr << "waiting for next client" << endl;
		int descriptor = accept(sockfd, addr, &addrlenl);
		if (descriptor < 0) {
			ERROR_OUT
			exit(1);
		}

		nodes.push_back(NetworkNode(descriptor));
		cout << "connected: " << nodes.size() << endl;
		if (nodes.size() == diplo::nbOfClients) {
			cerr << "All clients connected" << endl;
			return; //FIXME, this is not right way
		}
	}

}

void Server::ReceiveFromAllSelect(vector<KEY_ID> *results) {
	fd_set rfds;
	int retval, maxfd;
	unordered_set<int> fds_done;
	while (1) {
//		cout << "Server::ReceiveFromAllSelect: " << diplo::stopwatch_get() << endl;
		FD_ZERO(&rfds);
		maxfd = 2;
		for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
			if (fds_done.find(i) == fds_done.end()) {
				FD_SET(nodes[i].fd, &rfds);
				maxfd = max(nodes[i].fd, maxfd) + 1;
			}
		}
//		cout << "before select: " << diplo::stopwatch_get() << endl;
		retval = select(maxfd, &rfds, NULL, NULL, NULL);
//		cout << "after select: " << diplo::stopwatch_get() << endl;

		if (retval == -1)
			perror("select()");
		else if (retval) {
			for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
				if (FD_ISSET(nodes[i].fd, &rfds)) {
//					cout << "ready to receive from: " << i << "\t" << diplo::stopwatch_get() << endl;
					size_t s = 0;
					diplo::SRV.Receive((char*) &s, sizeof(s), i);
//					cout << "s: " << i << "\t" << diplo::stopwatch_get() << endl;
					size_t n = results->size();
					results->resize(n + s);
//					cout << "resize: " << i << "\t" << diplo::stopwatch_get() << endl;
					diplo::SRV.Receive((char *) (results->data() + n), s * sizeof(KEY_ID), i);
					fds_done.insert(i);
//					cout << "received from: " << i << "\t" << diplo::stopwatch_get() << endl;
				}
			}
		} else
			cerr << "No data" << endl;

		if (fds_done.size() == diplo::SRV.GetNbOfNodes())
			break;
	}

}

Server::~Server() {
	// TODO Auto-generated destructor stub
}

NetworkNode::NetworkNode(int _fd) {
	fd = _fd;
}

void NetworkNode::Send(const char* buf, size_t size) {

	size_t len = 0;
	while (len < size) {
		ssize_t n = write(fd, (buf + len), size - len);
		if (n <= 0) {
			cerr << "NetworkNode::Receive: " << endl;
			ERROR_OUT
			perror("NetworkNode::Send: ");
		}
		len += n;

	}

//	if ( write(fd,buf, size) != size)
//		cerr << "NetworkNode::Send" << endl;

}

void NetworkNode::Receive(char* buf, size_t size) {
	size_t len = 0;
	while (len < size) {
		size_t n = read(fd, (buf + len), size - len);
		if (n <= 0) {
			cerr << "NetworkNode::Receive: " << endl;
			ERROR_OUT
			exit(1);
		}
		len += n;

	}

//
//	size_t n = read(fd,buf, size);
//	if ( n != size)
//		cerr << "NetworkNode::Receive: expected: " <<  size << ", received:  " << n << endl;
}

NetworkNode::~NetworkNode() {

}
