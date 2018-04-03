#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
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

int hostname_to_ip(std::string hostname, std::string ip) {
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ((he = gethostbyname(hostname)) == NULL) {
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for (i = 0; addr_list[i] != NULL; ++i) {
        // Return the first one
        ip = inet_ntoa(*addr_list[i]);
        return 0;
    }
    return 0;
}

int query(const std::string& server, const std::string& query, const std::string *response) {
    std::string ip, message, buffer;
    int sock, read_size, total_size = 0;

    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;

    std::cout << "Resolving " << server << "..." << std::endl;
    
}

void get_whois(std::string ip, std::string **data) {
    std::string wch = NULL, pch, response = NULL;

    if (query("whois.iana.org", ip, &response)) {
        std::cout << "Whois query failed" << std::endl;
    }

    pch = strtok(response, "\n");

    while (pch != NULL) {
       wch = strstr(pch, "whois.");
       if (wch != NULL) {
            break;
       } 
    }
}

bool Harvester::harvest_addr(std::string address) {
    return true;    
}


/**
 * std::tuple<int, std::string> query(const std::string& server, const std::string& query) {
 *
 * }
 *
 * int new_main() {
 *     auto response = query("blah", "my query");
 *     std::cout << std::get<std::string>(response) << std::endl;
 * }
 */
