/**
 * @file tcp_socket.cpp
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Create send and recieve tcp sockets.
 * @date October 2019
 */

#include "tcp_socket.h"

#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

void TcpSocket::connectServer(std::string serverHostName, int serverPort)
{
	dst_addr = "host=\"" + serverHostName+ "\", port=\""+ std::to_string(serverPort) + "\"";
	struct hostent* server;
	if (!(server = gethostbyname(serverHostName.c_str())))
	{
		throw std::runtime_error("error: Can not create TCP connection to " + dst_addr);
	}

	//init server addr
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	in_addr *addr = (struct in_addr *) server->h_addr_list[0];
	char *ip = inet_ntoa(*addr);

	if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
	{
		throw std::runtime_error("error: Can not create TCP connection to " + dst_addr);
	}
	server_addr.sin_port = htons(serverPort);

	//create socket
	if ((socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
	{
		close(socket_id);
		throw std::runtime_error("error: Can not create TCP connection to " + dst_addr);
	}

	if (connect(socket_id, (const struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
	{
		close(socket_id);
		throw std::runtime_error("error: Can not create TCP connection to " + dst_addr);
	}
}

void TcpSocket::sendData(std::string data)
{
	if (send(socket_id, data.c_str(), data.length(), 0) < 0)
	{
		close(socket_id);
		throw std::runtime_error("error: Can not send data through TCP connection " + dst_addr);
	}
}

std::string TcpSocket::recvData()
{
	std::string data;
	unsigned size = 4096;
	char tmp[size];

	while (recv(socket_id, tmp, size, 0) > 0)
	{
		data += tmp;
		bzero(tmp, size);
	}

	return data;
}

void TcpSocket::closeConnection()
{
	close(socket_id);
}
