/**
 * @file dns_query.cpp
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Send DNS query and deparse response data with lreslov library.
 * @date October 2019
 */

#include "dns_query.h"

#include <arpa/inet.h>
#include <cstring>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/nameser.h>
#include <cstdio>

DnsQuery::DnsQuery(address_t dnsServer, address_t analyzedDomain)
{
	this->dnsServer = dnsServer;
	this->analyzedDomain = analyzedDomain;
}

//https://docstore.mik.ua/orelly/networking_2ndEd/dns/ch15_02.htm
//https://git.busybox.net/busybox/plain/networking/nslookup.c
void DnsQuery::askServer()
{
	res_init();
	_res.options &= ~(RES_DNSRCH | RES_DEFNAMES);
	_res.options |= RES_RECURSE;
	_res.retry = 2;
	_res.nscount = 1;

	if (!(server = gethostbyname(dnsServer.ipv4.c_str())))
	{
		throw std::runtime_error("error: gethostbyname");
	}
	memcpy(&_res.nsaddr_list[0].sin_addr, server->h_addr_list[0], server->h_length);

	ns_msg messageHandler;
	for (int i = 0; i < queryType.size(); ++i)
	{
		queryLength = res_mkquery(
			ns_o_query,
			analyzedDomain.hostname.c_str(),
			ns_c_in,
			queryType.at(i).first,
			NULL,
			0,
			NULL,
			(u_char *)&query,
			sizeof(query)
		);

		if ((answerLength = res_send((u_char*)&query, queryLength, (u_char*)&answer, sizeof(answer))) < 0)
		{
			throw std::runtime_error("error: res_send");
		}

		if ((ns_initparse(answer.buffer, answerLength, &messageHandler)) < 0)
		{
			throw std::runtime_error("error: ns_initparse");
		}
		if (ns_msg_getflag(messageHandler, ns_f_rcode) != ns_r_noerror)
		{
			continue; // dns reply -> field not found -> skip
		}
		ns_rr parsedRecord;
		for (int j = 0; j < ns_msg_count(messageHandler, ns_s_an); ++j)
		{
			if (ns_parserr(&messageHandler, ns_s_an, j, &parsedRecord))
			{
				throw std::runtime_error("error: ns_parserr");
			}
			if (ns_rr_type(parsedRecord) != queryType.at(i).first)
			{
				throw std::runtime_error("error: Did not recieve " + queryType.at(i).second);
			}
			printResultOFDnsQueries(queryType.at(i), parsedRecord, messageHandler);
		}
	}
}

// https://git.busybox.net/busybox/plain/networking/nslookup.c
void DnsQuery::printResultOFDnsQueries(
	std::pair<__ns_type, std::string> queryType,
	ns_rr parsedRecord,
	ns_msg messageHandler)
{
	u_char *dataPtr = (u_char *) ns_rr_rdata(parsedRecord);
	u_char *dataBegin = (u_char *)ns_msg_base(messageHandler);
	u_char *dataEnd = (u_char*) ns_msg_end(messageHandler);
	char domain[MAXDNAME];
	char ipAddr[INET6_ADDRSTRLEN];
	int n;

	std::string result;
	switch (queryType.first)
	{
		case ns_t_aaaa:
			inet_ntop(AF_INET6, dataPtr, ipAddr, INET6_ADDRSTRLEN);
			result = ipAddr;
			break;
		case ns_t_a:
			inet_ntop(AF_INET, dataPtr, ipAddr, INET_ADDRSTRLEN);
			result = ipAddr;
			break;
		case ns_t_soa:
			n = ns_name_uncompress(dataBegin, dataEnd, dataPtr, domain, MAXDNAME);
			result = "origin " + std::string(domain) + "\n";
			n = ns_name_uncompress(dataBegin, dataEnd, (dataPtr += n), domain, MAXDNAME);
			result += "SOA:\t\t\temail "   + std::string(domain) + "\n";
			result += "SOA:\t\t\tserial "  + std::to_string(ns_get32(dataPtr += n)) + "\n";
			result += "SOA:\t\t\trefresh " + std::to_string(ns_get32(dataPtr += 4)) + "\n";
			result += "SOA:\t\t\tretry "   + std::to_string(ns_get32(dataPtr += 4)) + "\n";
			result += "SOA:\t\t\texpire "  + std::to_string(ns_get32(dataPtr += 4)) + "\n";
			result += "SOA:\t\t\tminimum " + std::to_string(ns_get32(dataPtr));
			break;
		case ns_t_ns:
		case ns_t_cname:
		case ns_t_ptr:
			ns_name_uncompress(dataBegin, dataEnd, dataPtr, domain, MAXDNAME);
			result = domain;
			break;
		case ns_t_txt:
			bzero(domain, MAXDNAME);
			memcpy(domain, dataPtr + 1, *dataPtr);
			result = "\"" + std::string(domain) + "\"";
			break;
		case ns_t_mx:
			u_int32_t mx = ns_get16(dataPtr);
			ns_name_uncompress(dataBegin, dataEnd, dataPtr + 2, domain, MAXDNAME);
			result = std::to_string(mx) + " " + domain;
			break;
	}
	if (!result.empty())
	{
		printf("%-15s %s\n", queryType.second.c_str(), result.c_str());
	} else
	{
		std::cout << "no entries found\n";
	}
}
