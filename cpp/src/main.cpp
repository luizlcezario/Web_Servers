#include <iostream>
#include <fstream>
#include <cstdio>
#include <Configuration.hpp>
#include <SocketServer.hpp>
#include <WebServer.hpp>
#define MAX_EVENTS 10


int main(int argc, char *argv[])
{
	Config::Configuration config;
	WebServer webserver;

	std::string path = "./configuration/server.toml";
	if (argc != 2)
	{
		FILE *file = std::fopen(path.c_str(), "r");
		if (file == NULL)
		{
			std::cerr << "Usage: " << argv[0] << " <filename>.toml" << std::endl
					  << "Usage optional: save the file in the same directory as the executable in the folder configuration as 'server.config'" << std::endl;
			return 1;
		}
		std::fclose(file);
	}
	else
		path = argv[1];
	try
	{
		std::cout << "Loading file: " << path << std::endl;
		config.loadFile(path);
		std::cout << config;
		webserver = config.createSockets();
		std::cout << "Start Server:" << path << std::endl;
		webserver.start();
		
		
		// while (true)
		// {
		// 	int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS , -1);
		// 	if (num_events == -1)
		// 	{
		// 		std::cerr << "Failed to wait for events" << std::endl;
		// 		break;
		// 	}
		// 	for (int i = 0; i < num_events; ++i)
		// 	{
		// 		if (events[i].data.fd == listen_fd)
		// 		{
		// 			// New connection
		// 			struct sockaddr_in client_addr = {};
		// 			socklen_t client_addr_len = sizeof(client_addr);
		// 			int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		// 			if (client_fd == -1)
		// 			{
		// 				std::cerr << "Failed to accept connection" << std::endl;
		// 				continue;
		// 			}

		// 			// Add the new client socket to the epoll set
		// 			struct epoll_event client_event = {};
		// 			client_event.events = EPOLLIN;
		// 			client_event.data.fd = client_fd;

		// 			if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event) == -1)
		// 			{
		// 				std::cerr << "Failed to add client socket to epoll set" << std::endl;
		// 				close(client_fd);
		// 				continue;
		// 			}
		// 		}
		// 		else
		// 		{
		// 			// Handle events on connected sockets
		// 			int client_fd = events[i].data.fd;
		// 			std::cout << "Event on client socket " << client_fd << std::endl;
		// 			char buffer[1024];
		// 			memset(buffer, 0, sizeof(buffer));
		// 			int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
		// 			if (bytes_received < 0)
		// 			{
		// 				std::cerr << "Failed to receive data from client" << std::endl;
		// 			}
		// 			else if (bytes_received == 0)
		// 			{
		// 				std::cout << "Client disconnected" << std::endl;
		// 			}
		// 			else
		// 			{
		// 				std::cout << "Received: " << buffer << std::endl;
		// 				const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello, World!";
		// 				int bytes_sent = send(client_fd, response, strlen(response), 0);
		// 				if (bytes_sent < 0)
		// 				{
		// 					std::cerr << "Failed to send data to client" << std::endl;
		// 				}
		// 				else
		// 				{
		// 					std::cout << "Sent: " << response << std::endl;
		// 				}
		// 			}
		// 			close(client_fd);
		// 			continue;
		// 			// Handle incoming data on the client socket
		// 			// For example, read the HTTP request and send the response
		// 		}
		// 	}
		// }

		// close(epoll_fd);
		// return 0;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}