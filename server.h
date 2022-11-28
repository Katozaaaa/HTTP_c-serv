#pragma once
#include <string>

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "print_result.h"
#include "status_code.h"

class IServer {
protected:
	virtual bool startServer() = 0;
	virtual bool connectClient() = 0;
	virtual void closeConnection() = 0;
	virtual const int acceptMessage() = 0;
	virtual const char* getMessage() = 0;
	virtual const int sendMessage(const std::string&) = 0;
};

class Server : public IServer {
private:
	WSADATA wsaData;
	addrinfo* addr;
	SOCKET listen_socket;
	SOCKET client_socket;
	static const int data_size = 1024;
	char data[data_size];
private:
	void _WSAStartup();
	void _getaddrinfo();
	void _socket();
	void _bind();
	void _listen();
	void _accept();
	int _recv();
	int _send(const std::string&);
public:
	Server() = default;
	~Server();
	virtual bool startServer() noexcept;
	virtual bool connectClient() noexcept;
	virtual void closeConnection() noexcept;
	virtual const int acceptMessage();
	virtual const char* getMessage();
	virtual const int sendMessage(const std::string& response);
};