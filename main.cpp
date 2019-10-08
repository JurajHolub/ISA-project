/**
 * @file main.cpp
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief School project for VUT FIT Brno, subject ISA. Network service Whois questioner.
 * @date October 2019
 */

#include "dns_query.h"
#include "whois_query.h"
#include "argument_parser.h"
#include "geolocation_database.h"

int main(int argc, char **argv)
{
	ArgumentParser argumentParser;
	if (!argumentParser.parseArgs(argc, argv))
	{
		argumentParser.printHelp();
		return EXIT_SUCCESS;
	}

	auto analyzedDomain = argumentParser.getAnalyzedDomain();
	auto dnsServer = argumentParser.getDnsServer();
	auto whoisServer = argumentParser.getWhoisServer();

	DnsQuery dnsQuery(dnsServer, analyzedDomain);
	WhoisQuery whoisQuery(whoisServer, analyzedDomain);
	GeolocationDatabase geolocationDatabase(analyzedDomain.ipv4);

	try
	{
		std::cout <<   "==============================---------DNS--------==============================\n";
		dnsQuery.askServer();

		std::cout << "\n==============================--------WHOIS-------==============================\n";
		whoisQuery.askServer();

		std::cout << "\n==============================-GEOLOCATION SERVER-==============================\n";
		geolocationDatabase.askServer();
	}
	catch (std::runtime_error e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}