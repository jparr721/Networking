struct addrinfo {
	int 						ai_flags; // AI_PASSIVE, AI_CANONNAME, etc.
	int 						ai_family; // AF_INET, AF_INRT6, AF_UNSPEC specifies which Ip version you want to use
	int 						ai_socktype; // SOCK_STREAM, SOCK_DGRAM (tcp or udp)
	int 						ai_protocol; // use 0 for "any"
	size_t 					ai_addrlen; // size of ai_addr in bytes
	struct sockaddr *ai_addr; // struct sockaddr_in or _in6
	char 						*ai_canonname; // full canonical hostname

	struct addrinfo *ai_next; // linked list, next node
}

struct sockaddr {
	unsigned short		sa_family;		// address family, AF_xxx (likely AF_INET or AF_INET6)
	char 							sa_data[14];	// 14 bytes of protocol address
}

