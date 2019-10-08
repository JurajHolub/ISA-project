/**
 * @file geolocation_database.h
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Create HTTP request to ip-api.com geolocation database and parse recieved data.
 * @date October 2019
 */

#ifndef PROJECT_GEOLOCATION_DATABASE_H
#define PROJECT_GEOLOCATION_DATABASE_H

#include <string>

class GeolocationDatabase {
public:
	GeolocationDatabase(std::string analyzedDomain);
	void askServer();
	void printDeparsedData(std::string& jsonData);

private:
	std::string analyzedDomain;
};


#endif //PROJECT_GEOLOCATION_DATABASE_H
