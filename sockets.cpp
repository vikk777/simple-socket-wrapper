#include <unistd.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <cstring>

#include "sockets.h"

// Base class

Sockets_Base::Sockets_Base(unsigned short int port):
	m_sockfd{ socket(AF_INET, SOCK_STREAM | O_NONBLOCK, 0) },
	m_buf_size{ SOCK_BUF_SIZE }
{
	if (m_sockfd == -1) { perror("socket"); exit(errno); }

	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_port = htons(port);
	m_sockaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	m_buffer = new char[m_buf_size];

	std::cout << "socket build\n";
}

Sockets_Base::~Sockets_Base(){
	if (close(m_sockfd) == -1){ perror("close"); }

	delete[] m_buffer;

	std::cout << "socket closed\n";
}

int Sockets_Base::write(const char* buffer, sockfd_t sockfd)
{
	return send(sockfd, buffer, strlen(buffer), 0);
}

int Sockets_Base::read(std::string& buffer, sockfd_t sockfd)
{
	if (m_buf_size < buffer.capacity())
	{
		m_buf_size = buffer.capacity();

		delete[] m_buffer;
	
		m_buffer = new char[m_buf_size];
	}

	int len;

	if ((len = recv(sockfd, m_buffer, m_buf_size, 0)) <= 0)
		{ return -1; }

	if (len < m_buf_size) { m_buffer[len] = '\0'; }

	buffer = m_buffer;

	return len;
}

// Server class ------------------------

Sockets_Serv::Sockets_Serv(unsigned short int port):
	Sockets_Base(port),
	m_flag_stop{false},
	m_current_client{0}
{
	int opt = 1;

    if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) == -1) 
    	perror("setsockopt");

	if (bind(m_sockfd, (sockaddr*) &m_sockaddr, sizeof(m_sockaddr)) == -1){
		perror("bind"); exit(errno); }

	std::cout << "server binded\n";
}

Sockets_Serv::~Sockets_Serv(){
	for (int i = 0; i < m_sock_client.size(); ++i)
	{
		close(m_sock_client.at(i));
	}

	std::cout << "client socket closed\n";
}

int Sockets_Serv::open()
{
	if (listen(m_sockfd, SOCK_BACKLOG) == -1){ perror("listen"); return -1; }

	std::cout << "server is listening\n";

	do{
		int new_sock;

		if ((new_sock = accept(m_sockfd, nullptr, nullptr)) != -1)
		{
			if (fcntl(new_sock, F_SETFL, O_NONBLOCK) == -1) perror("fcntl");

			std::cout << " new connection id: " << new_sock << "\n";

			m_sock_client.push_back(new_sock);
		}

		else {				
			if (errno != EAGAIN){
				perror("accept"); exit(errno); }
		}

		sleep(2);
		
	} while (m_flag_stop != true);

	return 0;
}

int Sockets_Serv::write(const char* buffer)
{
	int len{-1};

	if (m_sock_client.size() == 0) { return len; }

	return Sockets_Base::write(buffer, m_sock_client.at(m_current_client));
}

int Sockets_Serv::write(const std::string& buffer)
{
	return Sockets_Serv::write(buffer.c_str());
}


int Sockets_Serv::read(std::string& buffer)
{
	int len{-1};

	if (m_sock_client.size() == 0) { return len; }

	return Sockets_Base::read(buffer, m_sock_client.at(m_current_client));
}

int Sockets_Serv::next_client()
{
	if (m_sock_client.size() > 0)
		++m_current_client %= m_sock_client.size();

	return m_current_client;
}

int Sockets_Serv::current_client()
{
	return m_current_client;
}

void Sockets_Serv::stop_listen()
{
	m_flag_stop = true;
}

void Sockets_Serv::delete_client()
{
	close(m_sock_client.at(m_current_client));

	m_sock_client.erase(m_sock_client.begin() + m_current_client);

	return;
}

// Client class ------------------------

Sockets::Sockets(unsigned short int port):
	Sockets_Base(port)
{
	do{
		if (connect(m_sockfd, (sockaddr*) &m_sockaddr, sizeof(m_sockaddr)) == 0){
			std::cout << "connetion established\n"; break;
		}
		else if (errno != EINPROGRESS){
			perror("connection"); exit(errno); }

	} while (true);
}

Sockets::~Sockets(){}

int Sockets::write(const std::string& buffer)
{
	return Sockets_Base::write(buffer.c_str(), m_sockfd);
}

int Sockets::read(std::string& buffer)
{
	return Sockets_Base::read(buffer, m_sockfd);
}
