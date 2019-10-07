/**
 * @file tcp_socket.h
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Create send and recieve tcp sockets.
 * @date October 2019
 */

#ifndef PROJECT_TCP_SOCKET_H
#define PROJECT_TCP_SOCKET_H

#include <iostream>

class TcpSocket {
public:

	void connectServer(std::string server, int serverPort);
	void sendData(std::string data);
	std::string recvData();
	void closeConnection();

private:
	int socket_id;
};


#endif //PROJECT_TCP_SOCKET_H

