#pragma once

class Server {
private:
		struct SlaveNode {
			const void *os_type;
		};
public:
	void* get_in_addr(struct sockaddr*);
	int server_loop();
};
