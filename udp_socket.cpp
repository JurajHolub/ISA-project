//
// Created by juraj on 30/09/2019.
//

#include "udp_socket.h"

#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

void UdpSocket::createSocket(std::string serverHostName, int serverPort)
{
	struct hostent* server;
	if (!(server = gethostbyname(serverHostName.c_str())))
	{
		throw std::runtime_error("error: gethostbyname(\"" + serverHostName + "\")");
	}

	//init server addr
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	in_addr *addr = (struct in_addr *) server->h_addr_list[0];
	char *ip = inet_ntoa(*addr);

	if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
	{
		throw std::runtime_error("error: inet_pton");
	}
	server_addr.sin_port = htons(serverPort);

	//create socket
	if ((socket_id = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
	{
		close(socket_id);
		throw std::runtime_error("error: socket(UDP)");
	}
}

void UdpSocket::sendData(std::string data)
{
	if (sendto(socket_id, data.c_str(), data.length(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		close(socket_id);
		throw std::runtime_error("error: sendto(UDP)");
	}
}

std::string UdpSocket::recvData()
{
	std::string data = "";
	unsigned size = 4096;
	char tmp[size];
	socklen_t server_len = sizeof(server_addr);

	while (recvfrom(socket_id, tmp, size, 0, (struct sockaddr*)&server_addr, &server_len) > 0)
	{
		data += tmp;
		bzero(tmp, size);
	}

	return data;
}
