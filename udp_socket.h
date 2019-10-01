//
// Created by juraj on 30/09/2019.
//

#ifndef PROJECT_UDP_SOCKET_H
#define PROJECT_UDP_SOCKET_H

#include <iostream>
#include <netinet/in.h>

class UdpSocket {

public:
	void createSocket(std::string serverHostName, int serverPort);
	void sendData(std::string data);
	std::string recvData();

private:
	int socket_id;
	struct sockaddr_in server_addr;
};


#endif //PROJECT_UDP_SOCKET_H
