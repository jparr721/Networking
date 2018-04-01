#pragma once
#include <string>

class Harvester {
		private:
				inline bool verify_harvester();
				void get_whois_data(char*, char**);
				int execute_whois_query(char*, char*, char**);
		public:
				bool harvest_addr(std::string);
};
