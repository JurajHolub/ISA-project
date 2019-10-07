/**
 * @file whois_deparser.h
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Deparse raw data recieved from whois query.
 * @date October 2019
 */

#ifndef PROJECT_WHOIS_QUERY_H
#define PROJECT_WHOIS_QUERY_H

#include <iostream>
#include <vector>
#include "argument_parser.h"

class WhoisQuery {
public:
	WhoisQuery(address_t whoisServer, address_t analyzedDomain);
	void printDeparsedData();
	void askServer();

private:
	address_t whoisServer;
	address_t analyzedDomain;
	std::string rawData;
	std::string parsedData;
	const std::vector<std::string> objects = {
		"inetnum",
		"netname",
		"descr",
		"country",
		"address",
		"phone",
		"admin-c"
	};
};


#endif //PROJECT_WHOIS_QUERY_H
