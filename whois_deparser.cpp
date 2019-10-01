/**
 * @file whois_deparser.cpp
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Deparse raw data recieved from whois query.
 * @date October 2019
 */

#include <sstream>
#include "whois_deparser.h"

WhoisDeparser::WhoisDeparser(std::string data)
{
	this->rawData = data;
}

void WhoisDeparser::printDeparsedData()
{
	std::istringstream iss(rawData);
	std::string line;
	std::vector<std::string> lines;

	while (std::getline(iss, line, '\n'))
	{
		lines.push_back(line);
	}

	for (auto object : objects)
	{
		for (auto parsedLine : lines)
		{
			if (parsedLine.rfind(object, 0) == 0)
			{
				parsedData += parsedLine + "\n";
			}
		}
	}

	std::cout << "=== WHOIS ===\n" << parsedData;
}