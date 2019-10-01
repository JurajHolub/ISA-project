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

class ArgumentParser {
public:
	ArgumentParser();
	bool parseArgs(int argc, char **argv);

	std::string getWhoisServer();
	std::string getHostname();
	std::string getDnsServer();

private:
	std::string whoisServer;
	std::string hostName;
	std::string dnsServer;
};


#endif //PROJECT_ARGUMENT_PARSER_H
