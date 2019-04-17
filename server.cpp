#include <iostream>
#include <thread>
#include <sys/utsname.h>

#include "sockets.h"
int a;
static bool t_flag_stop = false;

void thread_listen(Sockets_Serv& app)
{
	app.open();

	return;
}

void thread_recv(Sockets_Serv& app)
{
	std::string buffer;

	do
	{
		if (app.read(buffer) != -1)
		{
			std::cout << "Client " << app.current_client() << ": " << buffer << "\n";

			if (buffer == "exit")
			{
				app.delete_client();

					std::cout << "Connection refused by client " 
							  << app.current_client() << "\n";

			}else if (buffer == "uname")
				{
					utsname sys_info;

					if (uname(&sys_info) == -1) perror("uname");

					std::string str_to_write = sys_info.sysname;
								str_to_write += "\n";
								str_to_write += sys_info.nodename;
								str_to_write += "\n";
								str_to_write += sys_info.release;
								str_to_write += "\n";
								str_to_write += sys_info.version;
								str_to_write += "\n";
								str_to_write += sys_info.machine;
					
					app.write(str_to_write);
				}else{
					app.write("Unknown command");
				}
		}
		
		app.next_client();

	} while (t_flag_stop != true);

	return;
}

int main()
{
	Sockets_Serv serv;

	std::thread t_listen(thread_listen, std::ref(serv));

	std::thread t_recv(thread_recv, std::ref(serv));

	getchar();

	t_flag_stop = true;

	t_recv.join();

	serv.stop_listen();

	t_listen.join();

	return 0;
}