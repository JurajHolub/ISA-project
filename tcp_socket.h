//
// Created by juraj on 30/09/2019.
//

#ifndef PROJECT_TCP_SOCKET_H
#define PROJECT_TCP_SOCKET_H

#include <iostream>

class TcpSocket {

public:
	void connectServer(std::string serverHostName, int serverPort);
	void sendData(std::string data);
	std::string recvData();
	void closeConnection();

private:
	int socket_id;
};


#endif //PROJECT_TCP_SOCKET_H

