#pragma once

class Client {
public:
		void* get_in_addr(struct sockaddr*);
		int connection_loop();
};
