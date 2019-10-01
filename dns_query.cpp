//
// Created by juraj on 01/10/2019.
//

#include "dns_query.h"

#include <resolv.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/nameser.h>
#include <cstdio>

std::string DnsQuery::getDnsServer()
{
	char stringIp[INET_ADDRSTRLEN];
	bzero(stringIp, INET_ADDRSTRLEN);
	sockaddr_in dnsServerAddr = _res.nsaddr_list[0];
	inet_ntop(AF_INET, &dnsServerAddr.sin_addr, stringIp, INET_ADDRSTRLEN);
	return stringIp;
}

//https://docstore.mik.ua/orelly/networking_2ndEd/dns/ch15_02.htm
//https://git.busybox.net/busybox/plain/networking/nslookup.c
void DnsQuery::foo()
{
	res_init();
	std::string dnsServer = getDnsServer();

	union {
		HEADER header;
		u_char buffer[NS_PACKETSZ];
	} query, answer;
	int queryLength, answerLength;

	_res.options &= ~(RES_DNSRCH | RES_DEFNAMES);
	_res.options |= RES_RECURSE;
	_res.retry = 2;
	_res.nscount = 1;

	struct hostent* server;
	if (!(server = gethostbyname(dnsServer.c_str())))
	{
		throw std::runtime_error("error: gethostbyname");
	}
	memcpy(&_res.nsaddr_list[0].sin_addr, server->h_addr_list[0], server->h_length);

	__ns_type type[6] = {ns_t_aaaa, ns_t_soa, ns_t_a, ns_t_cname, ns_t_ns, ns_t_mx};
	std::string typeName[6] = {"AAAA", "SOA", "A", "CNAME", "NS", "MX"};

	for (int i = 0; i < 6; ++i)
	{
		queryLength = res_mkquery(
			ns_o_query,
			"isa.fit.vutbr.cz",
			ns_c_in,
			type[i],
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
			if (ns_rr_type(rr) != type[i])
			{
				throw std::runtime_error("error: Did not recieve SOA");
			}

			u_char *cp = (u_char*)ns_rr_rdata(rr);
			char dname[MAXDNAME];
			char astr[INET6_ADDRSTRLEN];

			std::string result = "";
			switch (type[i])
			{
				case ns_t_aaaa:
					inet_ntop(AF_INET6, cp, astr, sizeof(astr));
					result = astr;
					break;
				case ns_t_a:
					inet_ntop(AF_INET, cp, astr, sizeof(astr));
					result = astr;
					break;
				case ns_t_soa:
				case ns_t_ns:
				case ns_t_cname:
					ns_name_uncompress(ns_msg_base(handle), ns_msg_end(handle), cp, dname, sizeof(dname));
					result = dname;
					break;
				case ns_t_mx:
					u_int32_t mx = ns_get16(cp);
					ns_name_uncompress(ns_msg_base(handle), ns_msg_end(handle), cp + 2, dname, sizeof(dname));
					result = std::to_string(mx) + " " + dname;
			}
			//if (!answer.header.aa)
			//{
			//	std::cout << "Non-authoritative answer:\n";
			//}
			if (!result.empty())
			{
				printf("%-15s %s\n", typeName[i].c_str(), result.c_str());
			}
		}
	}
}
