/**
 * @file dns_query.h
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Send DNS query and deparse response data with lreslov library.
 * @date October 2019
 */

#ifndef PROJECT_DNS_QUERY_H
#define PROJECT_DNS_QUERY_H

#include <iostream>
#include <resolv.h>
#include <vector>

class DnsQuery {
public:
	DnsQuery(std::string dnsServer);
	void askServer(std::string analyzedDomain);
	void findDnsServer();
	void printResultOFDnsQueries(int i, ns_rr rr, ns_msg handle);

private:
	union {
		HEADER header;
		u_char buffer[NS_PACKETSZ];
	} query, answer;
	std::string dnsServer;
	struct hostent* server;
	int queryLength, answerLength;
	const std::vector<std::pair<__ns_type, std::string>> queryType = {
		{ns_t_aaaa, "AAAA:"},
		{ns_t_soa, "SOA:"},
		{ns_t_a, "A:"},
		{ns_t_cname, "CNAME:"},
		{ns_t_ns, "NS:"},
		{ns_t_mx, "MX:"},
		{ns_t_ptr, "PTR:"},
		{ns_t_txt, "TXT:"}
	};
};


#endif //PROJECT_DNS_QUERY_H
