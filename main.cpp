#include <iostream>
#include "server.h"
#include "response.h"
#include "file_reader.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "print_result.h"

int main() {
	Server server;
	if (server.startServer()) {

		for (int i = 0; i < 40; ++i)
		{
			std::cout << '_';
		}
		std::cout << "\n\n";

		while (true) {
			if (server.connectClient()) {
				std::stringstream response_stream;
				std::string resource;
				try {
					const int data_size = server.acceptMessage();
					if (data_size > 0) {
						const std::string data = server.getMessage();
						std::cout << '\n' << data << '\n';
						Request request((std::stringstream)data);
						resource = request.getURL();

						Response response(request);
						response_stream << response.getAll();

						std::cerr << "Status code: ";
						print_result(FOREGROUND_GREEN, OK);
					}
				}
				catch (const char* status) {
					response_stream.clear();
					response_stream << "HTTP/1.1 " << status << "\r\n";
					std::cerr << "Status code: ";
					print_result(FOREGROUND_RED, status);
				}
				catch (...) {
					response_stream.clear();
					response_stream << "HTTP/1.1 " << SERVER_ERROR << "\r\n";
					std::cerr << "Status code: ";
					print_result(FOREGROUND_RED, SERVER_ERROR);
				}
				try {
					server.sendMessage(response_stream.str());
					std::cerr << "Resource: " << resource << '\n';
				}
				catch (...) {
					print_result(FOREGROUND_RED, "\n\nError when sending message");
				}
			}

			server.closeConnection();
			for (int i = 0; i < 40; ++i) {
				std::cout << '_';
			}
			std::cout << "\n\n";
		}
	}
	else return 1;
	return 0;
}