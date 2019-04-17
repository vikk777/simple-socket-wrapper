#ifndef TSOCKETS
#define TSOCKETS

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>

// consts and typedefs

typedef int sockfd_t;

const int SOCK_BACKLOG = 1;
const int SOCK_BUF_SIZE = 15;

// Base class

class Sockets_Base
{
protected:
	int m_sockfd;
	int m_buf_size;

	char* m_buffer;

	sockaddr_in m_sockaddr;

	Sockets_Base(unsigned short int);

public:
	~Sockets_Base();

	int write(const char*, sockfd_t);

	int read(std::string&, sockfd_t);
};

// Server class -------------------------

class Sockets_Serv : public Sockets_Base
{
private:
	static const unsigned short int DEFAULT_PORT = 7777;

	std::vector<int> m_sock_client;
	bool m_flag_stop;
	int m_current_client;

public:
	Sockets_Serv(unsigned short int = DEFAULT_PORT);
	
	~Sockets_Serv();

	int open();
	
	int write(const char*);
	
	int write(const std::string&);

	int read(std::string&);

	int next_client();
	
	int current_client();

	void stop_listen();

	void delete_client();
};

// Client class -------------------------

class Sockets : public Sockets_Base
{
public:
	Sockets(unsigned short int);

	~Sockets();
	
	int write(const std::string&);

	int read(std::string&);
};

#endif