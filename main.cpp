/**
 * @file main.cpp
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief School project for VUT FIT Brno, subject ISA. Network service Whois questioner.
 * @date October 2019
 */


#include "tcp_socket.h"
#include "udp_socket.h"
#include "dns_query.h"

int main()
{
	DnsQuery dnsQuery;
	dnsQuery.foo();

	try {
	/*
		TcpSocket tcpSocket;
		tcpSocket.connectServer("whois.ripe.net", 43);
		tcpSocket.sendData("-B 147.229.2.90\r\n");
		std::cout << tcpSocket.recvData() << std::endl;
		tcpSocket.closeConnection();
		UdpSocket udpSocket;
		udpSocket.createSocket(getDnsServer(), 53);
		udpSocket.sendData("");
		std::cout << udpSocket.recvData() << std::endl;
	*/
	}
	catch (std::runtime_error &e) {
		e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}