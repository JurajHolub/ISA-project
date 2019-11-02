/**
 * @file argument_parser.h
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Parse command line arguments.
 * @date October 2019
 */

#include <iostream>

#ifndef PROJECT_ARGUMENT_PARSER_H
#define PROJECT_ARGUMENT_PARSER_H

#define WHOISE_SERVVER 0
#define HOSTNAME 1
#define DNS_SERVER 2
#define HELP_MSG 3

struct address_t {
	std::string hostname;
	std::string ipv4;
};

class ArgumentParser {
public:

	ArgumentParser();
	bool parseArgs(int argc, char **argv);
	void printHelp();

	address_t getWhoisServer();
	address_t getAnalyzedDomain();
	address_t getDnsServer();
	/**
	 * If input is IPV4 then find domain name.
	 * If input is domain name then find IPv4 address.
	 * If input is IPv6 then find domain name and by domain name find ipv4.
	 * Else dont fill ipv4 and domain and DNS or Whois server didnt found content (user input error).
	 * @return True if input is valid IP|hostname.
	 */
	bool parseDomain(std::string input, address_t &output);

private:
	address_t whoisServer;
	address_t analyzedDomain;
	address_t dnsServer;
};


#endif //PROJECT_ARGUMENT_PARSER_H
