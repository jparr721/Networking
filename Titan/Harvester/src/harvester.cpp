#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include "../include/harvester.hpp"

#define HARVESTED_DIR "../../../.harvested"
/**
 * Determines that a .harvested file exists
 */
inline bool Harvester::verify_harvester() {
	std::cout << "Verifying previous harvester instance..." << std::endl;
	std::ifstream file(HARVESTED_DIR);
	if (file.good()) {
		std::cout << ".harvested file found, proceeding" << std::endl;	
		return file.good();
	} else {
		std::cout << ".harvested file not found, loading a new one..." << std::endl;
		file.open(".harvested", std::fstream::out);	
	}
}

int Harvester::execute_whois_query(char* server, char* query, char** response) {

}

void Harvester::get_whois_data(char* ip, char** data) {
	char *wch = NULL, *pch, *response = NULL;

	if (execute_whois_query("whois.iana.org", ip, &response)) {
		std::cout << "Whois query failed, storing IP only" << std::endl;
	}
}

bool Harvester::harvest_addr(std::string address) {
	return true;	
}
