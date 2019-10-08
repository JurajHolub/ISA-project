/**
 * @file argument_parser.cpp
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Parse command line arguments.
 * @date October 2019
 */

#include "argument_parser.h"
#include <getopt.h>
#include <list>
#include <netdb.h>
#include <arpa/inet.h>

ArgumentParser::ArgumentParser()
{
	dnsServer.ipv4 = "1.1.1.1";
	dnsServer.hostname = "one.one.one.one";
}

bool ArgumentParser::parseArgs(int argc, char **argv)
{
	int opt;
	const struct option long_options[] = {
			{"q" , required_argument, 0, HOSTNAME},
			{"w" , required_argument, 0, WHOISE_SERVVER},
			{"d" , required_argument, 0, DNS_SERVER},
			{"h" , no_argument, 0, HELP_MSG}
	};
	std::list<std::string> argsLeft;
	for (int i = 1; i < argc; i++)
	{
		argsLeft.push_front(std::string(argv[i]));
	}
	bool domainDefined = false;
	bool whoisDefined = false;

	while ((opt = getopt_long_only(argc, argv, "", long_options, nullptr)) != -1)
	{
		switch (opt)
		{
			case HOSTNAME:
				domainDefined = true;
				analyzedDomain = parseDomain(optarg);
				argsLeft.remove("-q");
				argsLeft.remove(optarg);
				break;
			case WHOISE_SERVVER:
				whoisDefined = true;
				whoisServer = parseDomain(optarg);
				argsLeft.remove("-w");
				argsLeft.remove(optarg);
				break;
			case DNS_SERVER:
				dnsServer = parseDomain(optarg);
				argsLeft.remove("-d");
				argsLeft.remove(optarg);
				break;
			case HELP_MSG:
				return false; //just print help message
		}
	}

	return argsLeft.empty() && domainDefined && whoisDefined;
}

address_t ArgumentParser::parseDomain(std::string input)
{
	address_t output={.hostname="", .ipv4=""};
	struct hostent* domain;
	struct sockaddr_in tmpAddr;
	struct sockaddr_in6 tmpAddr6;
	char ipv4[INET_ADDRSTRLEN];
	char ipv6[INET6_ADDRSTRLEN];

	if (inet_pton(AF_INET6, input.c_str(), &tmpAddr6.sin6_addr)!=0) //possible ipv6 addr
	{
		inet_ntop(AF_INET6, &tmpAddr6.sin6_addr, ipv6, sizeof(ipv6));

		if ((domain=gethostbyaddr(&tmpAddr6.sin6_addr, sizeof(tmpAddr6.sin6_addr), AF_INET6)))
		{
			//now we know domain name
			output.hostname = domain->h_name;
		}

		//now we find ipv4 from domain name
		if ((domain = gethostbyname(output.hostname.c_str())))
		{
			tmpAddr.sin_addr = *(struct in_addr*)domain->h_addr_list[0];
			inet_ntop(AF_INET, &tmpAddr.sin_addr, ipv4, sizeof(ipv4));
			output.ipv4 = std::string(ipv4); //ipv4
		}
	}
	else if ((domain = gethostbyname(input.c_str()))) //handle validation of ipv4 or hostname
	{
		tmpAddr.sin_addr = *(struct in_addr*)domain->h_addr_list[0];
		inet_ntop(AF_INET, &tmpAddr.sin_addr, ipv4, sizeof(ipv4));

		output.ipv4 = std::string(ipv4); //ipv4

		if (output.ipv4 == input) //input is ipv4 so we still dont know domain
		{
			if ((domain=gethostbyaddr(&tmpAddr.sin_addr, sizeof(tmpAddr.sin_addr), AF_INET)))
			{
				//now we know domain name
				output.hostname = domain->h_name;
			}
		}
		else
		{
			output.hostname = input;
		}
	}
	else
	{
		output.hostname = input;
	}

	return output;
}

address_t ArgumentParser::getDnsServer()
{
	return dnsServer;
}

address_t ArgumentParser::getWhoisServer()
{
	return whoisServer;
}

address_t ArgumentParser::getAnalyzedDomain()
{
	return analyzedDomain;
}

void ArgumentParser::printHelp()
{
	std::cout
	<< "usage: isa-tazatel <arguments>" << std::endl
	<< "Compulsory arguments:" << std::endl
	<< "  -q <IP|hostname>  = IP or hostname address to by analyzed." << std::endl
	<< "  -w <IP|hostname>  = IP or hostname of questioned WHOIS server" << std::endl
	<< "Optional arguments:" << std::endl
	<< "  -d <IP|hostname>  = IP or hostanme of questioned DNS server" << std::endl
	<< "                     (default value \"1.1.1.1\")." << std::endl
	<< "  -h                = Print this help message." << std::endl;
}
/*
*/