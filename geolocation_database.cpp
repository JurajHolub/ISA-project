/**
 * @file geolocation_database.c
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Create HTTP request to ip-api.com geolocation database and parse recieved data.
 * @date October 2019
 */

#include "geolocation_database.h"
#include "tcp_socket.h"
#include <vector>

GeolocationDatabase::GeolocationDatabase(std::string analyzedDomain)
{
	this->analyzedDomain = analyzedDomain;
}

//https://ip-api.com/docs/api:json
void GeolocationDatabase::askServer()
{
	TcpSocket tcpSocket;
	tcpSocket.connectServer("ip-api.com", 80); // port 80 = HTTP
	tcpSocket.sendData(
		"GET /json/"+analyzedDomain+"?"
		"fields=continent,continentCode,country,countryCode,"
		"region,regionName,city,zip,timezone,currency,isp,org,"
		"as,asname,reverse,query HTTP/1.1\r\n"
		"Host: ip-api.com\r\n"
		"Connection: close\r\n"
		"Cache-Control: max-age=0\r\n"
		"Upgrade-Insecure-Requests: 1\r\n"
		"Accept: */*\r\n"
		"Accept-Encoding: gzip, deflate\r\n"
		"Accept-Language: en-GB\r\n"
		"\r\n"
	);
	std::string jsonData = tcpSocket.recvData();
	printDeparsedData(jsonData);
}

void GeolocationDatabase::printDeparsedData(std::string& jsonData)
{
	auto start = jsonData.find_first_of('{'); //start of json data
	jsonData = jsonData.substr(start + std::string("{\"").size(), jsonData.size());
	auto end = jsonData.find_last_of('}'); // end of json data
	jsonData = jsonData.substr(0, end - sizeof('}')); // cut

	std::string jsonItem;
	std::vector<std::string> lines;
	int delimPos;
	//split to lines
	while ((delimPos=jsonData.find("\",\"")) != std::string::npos)
	{
		jsonItem = jsonData.substr(0, delimPos);
		lines.push_back(jsonItem);
		jsonData.erase(0, delimPos + std::string("\",\"").size());
	}
	lines.push_back(jsonData);

	// print formated data
	int delim;
	for (auto line : lines)
	{
		delim = line.find("\":\"");
		auto lValue = line.substr(0, delim);
		auto rValue = line.substr(delim+3, line.size());
		if (!rValue.empty())
		{
			printf("%-16s%s\n", lValue.c_str(), rValue.c_str());
		}
	}
}