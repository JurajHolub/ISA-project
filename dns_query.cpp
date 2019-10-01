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

DnsQuery::DnsQuery(std::string dnsServer)
{
	this->dnsServer = dnsServer;
}

void DnsQuery::findDnsServer()
{
	char stringIp[INET_ADDRSTRLEN];
	bzero(stringIp, INET_ADDRSTRLEN);
	sockaddr_in dnsServerAddr = _res.nsaddr_list[0];
	inet_ntop(AF_INET, &dnsServerAddr.sin_addr, stringIp, INET_ADDRSTRLEN);
	dnsServer = stringIp;
}

//https://docstore.mik.ua/orelly/networking_2ndEd/dns/ch15_02.htm
//https://git.busybox.net/busybox/plain/networking/nslookup.c
void DnsQuery::askServer(std::string analyzedDomain)
{
	std::cout << "=== DNS ===\n";
	res_init();
	//findDnsServer();
	_res.options &= ~(RES_DNSRCH | RES_DEFNAMES);
	_res.options |= RES_RECURSE;
	_res.retry = 2;
	_res.nscount = 1;

	if (!(server = gethostbyname(dnsServer.c_str())))
	{
		throw std::runtime_error("error: gethostbyname");
	}
	memcpy(&_res.nsaddr_list[0].sin_addr, server->h_addr_list[0], server->h_length);

	for (int i = 0; i < queryType.size(); ++i)
	{
		queryLength = res_mkquery(
			ns_o_query,
			analyzedDomain.c_str(),
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

		ns_msg handle;
		if ((ns_initparse(answer.buffer, answerLength, &handle)) < 0)
		{
			throw std::runtime_error("error: ns_initparse");
		}
		if (ns_msg_getflag(handle, ns_f_rcode) != ns_r_noerror)
		{
			throw std::runtime_error("error: ns_initparse");
		}
		for (int j = 0; j < ns_msg_count(handle, ns_s_an); ++j)
		{
			ns_rr rr;
			if (ns_parserr(&handle, ns_s_an, j, &rr))
			{
				throw std::runtime_error("error: ns_parserr");
			}
			if (ns_rr_type(rr) != queryType.at(i).first)
			{
				throw std::runtime_error("error: Did not recieve " + queryType.at(i).second);
			}
			printResultOFDnsQueries(i, rr, handle);
		}
	}
}

void DnsQuery::printResultOFDnsQueries(int i, ns_rr rr, ns_msg handle)
{
	auto cp = (u_char*)ns_rr_rdata(rr);
	char dname[MAXDNAME];
	char addrStr[INET6_ADDRSTRLEN];
	int n;

	std::string result;
	switch (queryType.at(i).first)
	{
		case ns_t_aaaa:
			inet_ntop(AF_INET6, cp, addrStr, sizeof(addrStr));
			result = addrStr;
			break;
		case ns_t_a:
			inet_ntop(AF_INET, cp, addrStr, sizeof(addrStr));
			result = addrStr;
			break;
		case ns_t_soa:
			n = ns_name_uncompress(ns_msg_base(handle), ns_msg_end(handle), cp, dname, sizeof(dname));
			result += "origin " + std::string(dname) + "\n\t\t\t\t";
			n = ns_name_uncompress(ns_msg_base(handle), ns_msg_end(handle), (cp += n), dname, sizeof(dname));
			result += "email " + std::string(dname) + "\n\t\t\t\t";
			result += "serial " + std::to_string(ns_get32(cp += n)) + "\n\t\t\t\t";
			result += "refresh " + std::to_string(ns_get32(cp += 4)) + "\n\t\t\t\t";
			result += "retry " + std::to_string(ns_get32(cp += 4)) + "\n\t\t\t\t";
			result += "expire " + std::to_string(ns_get32(cp += 4)) + "\n\t\t\t\t";
			result += "minimum " + std::to_string(ns_get32(cp)) + "\n";
			break;
		case ns_t_ns:
		case ns_t_cname:
		case ns_t_ptr:
			ns_name_uncompress(ns_msg_base(handle), ns_msg_end(handle), cp, dname, sizeof(dname));
			result = dname;
			break;
		case ns_t_txt:
			bzero(dname, sizeof(dname));
			memcpy(dname, cp+1, *cp);
			result = "\"" + std::string(dname) + "\"";
			break;
		case ns_t_mx:
			u_int32_t mx = ns_get16(cp);
			ns_name_uncompress(ns_msg_base(handle), ns_msg_end(handle), cp + 2, dname, sizeof(dname));
			result = std::to_string(mx) + " " + dname;
			break;
	}
	if (!result.empty())
	{
		printf("%-15s %s\n", queryType.at(i).second.c_str(), result.c_str());
	}
}
