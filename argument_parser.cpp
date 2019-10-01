/**
 * @file argument_parser.cpp
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Parse command line arguments.
 * @date October 2019
 */

#include "argument_parser.h"
#include <getopt.h>
#include <list>

ArgumentParser::ArgumentParser()
{
	dnsServer = "1.1.1.1";
	whoisServer = "whois.ripe.net";
}

bool ArgumentParser::parseArgs(int argc, char **argv)
{
	int opt;
	const struct option long_options[3] = {
			{"q" ,required_argument, 0, HOSTNAME},
			{"w" ,required_argument, 0, WHOISE_SERVVER},
			{"d" , required_argument, 0, DNS_SERVER},
	};
	std::list<std::string> argsLeft;

	for (int i = 1; i < argc; i++)
	{
		argsLeft.push_front(std::string(argv[i]));
	}

	while ((opt = getopt_long_only(argc, argv, "", long_options, nullptr)) != -1)
	{
		switch (opt)
		{
			case HOSTNAME:
				hostName = std::string(optarg);
				argsLeft.remove("-h");
				argsLeft.remove(hostName);
				break;
			case WHOISE_SERVVER:
				whoisServer = std::string(optarg);
				argsLeft.remove("-w");
				argsLeft.remove(whoisServer);
				break;
			case DNS_SERVER:
				dnsServer = std::string(optarg);
				argsLeft.remove("-d");
				argsLeft.remove(dnsServer);
				break;
		}
	}

	return argsLeft.empty();
}

std::string ArgumentParser::getDnsServer()
{
	return dnsServer;
}

std::string ArgumentParser::getHostname()
{
	return hostName;
}

std::string ArgumentParser::getWhoisServer()
{
	return whoisServer;
}
