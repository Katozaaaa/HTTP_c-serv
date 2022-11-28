#include "server.h"
#include <iostream>

void Server::_WSAStartup() {
	std::cerr << "WSAStartup: ";
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		print_result(FOREGROUND_RED, result);
		throw false;
	}
	else {
		print_result(FOREGROUND_GREEN, SUCCESS);
	}
}

void Server::_getaddrinfo() {
	std::cerr << "Get addrinfo: ";
	addrinfo tmpl_addr;
	ZeroMemory(&tmpl_addr, sizeof(tmpl_addr));
	tmpl_addr.ai_family = AF_INET;
	tmpl_addr.ai_socktype = SOCK_STREAM;
	tmpl_addr.ai_protocol = IPPROTO_TCP;
	tmpl_addr.ai_flags = AI_PASSIVE;

	int result = getaddrinfo("127.0.0.1", "8000", &tmpl_addr, &addr);
	if (result != 0) {
		print_result(FOREGROUND_RED, result);
		throw false;
	}
	else {
		print_result(FOREGROUND_GREEN, SUCCESS);
	}
}

void Server::_socket() {
	std::cerr << "Socket Initialization: ";
	listen_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (listen_socket == INVALID_SOCKET) {
		print_result(FOREGROUND_RED, WSAGetLastError());
		throw false;
	}
	else {
		print_result(FOREGROUND_GREEN, SUCCESS);
	}
}

void Server::_bind() {
	std::cerr << "Bind: ";
	int result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);
	if (result == SOCKET_ERROR) {
		print_result(FOREGROUND_RED, result);
		throw false;
	}
	else {
		print_result(FOREGROUND_GREEN, SUCCESS);
	}
}

void Server::_listen() {
	std::cerr << "Listen: ";
	int result = listen(listen_socket, SOMAXCONN);
	if (result == SOCKET_ERROR) {
		print_result(FOREGROUND_RED, result);
		throw false;
	}
	else {
		print_result(FOREGROUND_GREEN, SUCCESS);
	}
}

void Server::_accept() {
	std::cerr << "Accept: ";
	client_socket = accept(listen_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET) {
		print_result(FOREGROUND_RED, WSAGetLastError());
		throw false;
	}
	else {
		print_result(FOREGROUND_GREEN, SUCCESS);
	}
}

int Server::_recv() {
	int result = recv(client_socket, data, data_size - 1, 0);
	if (result == SOCKET_ERROR) {
		std::cerr << "Recv failed: ";
		print_result(FOREGROUND_RED, WSAGetLastError());
		throw SERVER_ERROR;
	}
	else if (result > 0) {
		if (data_size == result) {
			std::cerr << "Recv failed: ";
			print_result(FOREGROUND_RED, "the client sent a large amount of data");
			throw PAYLOAD_TOO_LARGE;
		}
		else {
			std::cerr << "Recv is successful: ";
			print_result(FOREGROUND_GREEN, result);
			data[result++] = '\0';
		}
	}
	return result;
}

int Server::_send(const std::string& response) {
	int result = send(client_socket, response.c_str(), response.length(), 0);
	if (result == SOCKET_ERROR) {
		std::cerr << "Send failed: ";
		print_result(FOREGROUND_RED, WSAGetLastError());
	}
	else {
		std::cerr << "Send is successful: ";
		print_result(FOREGROUND_GREEN, result);
	}
	return result;
}

Server::~Server() {
	closesocket(listen_socket);
	freeaddrinfo(addr);
	WSACleanup();
	closeConnection();
}

bool Server::startServer() noexcept {
	try {
		_WSAStartup();
		_getaddrinfo();
		_socket();
		_bind();
		_listen();
	}
	catch (...) {
		print_result(FOREGROUND_RED, "\n\nError when starting the server");
		return 0;
	}
	return 1;
}
bool Server::connectClient() noexcept {
	try {
		_accept();
	}
	catch (...) {
		print_result(FOREGROUND_RED, "\n\nError when connecting the client");
		return 0;
	}
	return 1;
}
void Server::closeConnection() noexcept {
	closesocket(client_socket);
	print_result(FOREGROUND_YELLOW, "Connection closed");
}
const int Server::acceptMessage() {
	int result = _recv();
	if (result <= 0) {
		return 0;
	}
	else {
		return result;
	}
}
const char* Server::getMessage() {
	return data;
}
const int Server::sendMessage(const std::string& response) {
	int result = _send(response);
	return result;
}