/**
 * @file main.cpp
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief School project for VUT FIT Brno, subject ISA. Network service Whois questioner.
 * @date October 2019
 */


#include "tcp_socket.h"
#include "dns_query.h"
#include "whois_deparser.h"
#include "argument_parser.h"

int main(int argc, char **argv)
{
	ArgumentParser argumentParser;
	if (!argumentParser.parseArgs(argc, argv))
	{
		return EXIT_FAILURE;
	}

	try {
		DnsQuery dnsQuery(argumentParser.getDnsServer());
		dnsQuery.askServer(argumentParser.getHostname());

		TcpSocket tcpSocket;
		tcpSocket.connectServer(argumentParser.getWhoisServer(), 43);
		tcpSocket.sendData("-B "+ argumentParser.getHostname() +"\r\n");
		std::string data = tcpSocket.recvData();
		tcpSocket.closeConnection();

		WhoisDeparser whoisDeparser = WhoisDeparser(data);
		whoisDeparser.printDeparsedData();
	}
	catch (std::runtime_error &e) {
		e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}