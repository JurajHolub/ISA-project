/**
 * @file whois_deparser.h
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Deparse raw data recieved from whois query.
 * @date October 2019
 */

#ifndef PROJECT_WHOIS_DEPARSER_H
#define PROJECT_WHOIS_DEPARSER_H

#include <iostream>
#include <vector>

class WhoisDeparser {
public:
	WhoisDeparser(std::string data);
	void printDeparsedData();

private:
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


#endif //PROJECT_WHOIS_DEPARSER_H
