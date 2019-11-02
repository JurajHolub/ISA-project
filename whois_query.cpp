/**
 * @file whois_deparser.cpp
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Send whois query and deparse recieved data.
 * @date October 2019
 */

#include <sstream>
#include "whois_query.h"
#include "tcp_socket.h"

void WhoisQuery::askServer()
{
	//create tcp connection with whoice server and send whoice query
	TcpSocket tcpSocket;
	tcpSocket.connectServer(whoisServer.hostname, 43);// whois TCP service always on port 43
	tcpSocket.sendData(analyzedDomain.ipv4 + "\r\n");
	rawData = tcpSocket.recvData();
	tcpSocket.closeConnection();

	printDeparsedData();
}

WhoisQuery::WhoisQuery(address_t whoisServer, address_t analyzedDomain)
{
	this->whoisServer = whoisServer;
	this->analyzedDomain = analyzedDomain;
}

void WhoisQuery::printDeparsedData()
{
	std::istringstream iss(rawData);
	std::string line;
	std::vector<std::string> lines;

	while (std::getline(iss, line, '\n'))
	{
		lines.push_back(line);
	}

	for (auto object : objects)
	{
		for (auto parsedLine : lines)
		{
			if (parsedLine.rfind(object, 0) == 0)
			{
				parsedData += parsedLine + "\n";
			}
		}
	}

	if (parsedData.empty())
	{
		std::cout << "Whois server found no entries.\n";
	}
	else
	{
		std::cout << parsedData;
	}
}