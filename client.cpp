#include <iostream>
#include <thread>

#include "sockets.h"

static bool t_flag_stop = false;

void thread_recv(Sockets& app)
{
	std::string buffer;

	buffer.reserve(128);

	do
	{
		if (app.read(buffer) != -1){
			std::cout << "Server: " << buffer << "\n"; }

		std::this_thread::yield();

	} while (t_flag_stop != true);

	return;
}

void thread_send(Sockets& app)
{
	std::string buffer;

	do {
		std::getline(std::cin, buffer);

		app.write(buffer);

		std::this_thread::yield();

	} while (buffer != "exit");

	return;
}

int main()
{
	Sockets client(7777);

	std::thread t_recv(thread_recv, std::ref(client));

	std::thread t_send(thread_send, std::ref(client));

	t_send.join();

	t_flag_stop = true;

	t_recv.join();

	return 0;
}