#pragma once
#include <string>
#include <tuple>

class Harvester {
		private:
				inline bool verify_harvester();
                std::tuple<int, std::string> query(const std::string&, const std::string&);
		public:
				bool harvest_addr(std::string);
};
