/**
 * @file main.cpp
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief School project for VUT FIT Brno, subject ISA. Network service Whois questioner.
 * @date October 2019
 */


#include "dns_query.h"
#include "whois_query.h"
#include "argument_parser.h"

int main(int argc, char **argv)
{
	ArgumentParser argumentParser;
	if (!argumentParser.parseArgs(argc, argv))
	{
		argumentParser.printHelp();
		return EXIT_SUCCESS;
	}

	std::cout << "Analyzed domain: " << argumentParser.getAnalyzedDomain().hostname << std::endl;

	DnsQuery dnsQuery(argumentParser.getDnsServer(), argumentParser.getAnalyzedDomain());
	WhoisQuery whoisQuery(argumentParser.getWhoisServer(), argumentParser.getAnalyzedDomain());

	try {
		dnsQuery.askServer();
		whoisQuery.askServer();
	}
	catch (std::runtime_error e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}