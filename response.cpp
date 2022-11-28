#pragma once
#include "file_reader.h"
#include "request.h"
#include "response.h"
#include <string>

std::string getContentType(const std::string type) {
	std::stringstream content_type;
	if (type == "text/html")
		content_type << type << "; " << "charset=utf-8" << ";";
	else 
		content_type << type << ";";
	return content_type.str();
}

Response::Response(const Request& request) {
	start_line.version = "HTTP/1.1";
	start_line.status = OK;

	std::ifstream* file(openFile(request.getURL()));

	const std::string& accept = request.getHeaderValue("Accept");
	const std::string& data_format = request.getDataFormat();

	if (data_format == "html" && (accept.find("text/html") != std::string::npos || accept.find("text/*") != std::string::npos)) {
		headers["Content-Type"] = getContentType("text/html");
	}
	else if (data_format == "css" && (accept.find("text/css") != std::string::npos || accept.find("text/*") != std::string::npos)) {
		headers["Content-Type"] = getContentType("text/css");
	}
	else if (data_format == "jpg" && (accept.find("image/jpg") != std::string::npos || accept.find("image/*") != std::string::npos)) {
		headers["Content-Type"] = getContentType("image/jpg");
	}
	else if (data_format == "svg" && (accept.find("image/svg+xml") != std::string::npos || accept.find("image/*") != std::string::npos)) {
		headers["Content-Type"] = getContentType("image/svg+xml");
	}
	else if (data_format == "js" && accept.find("*/*") != std::string::npos) {
		headers["Content-Type"] = getContentType("*/*");
	}
	else throw NOT_ACCEPTABLE;
	body = readFile(file, data_format);
	headers["Content-Length"] = std::to_string(body.length());
}

const std::string Response::getStartLine() const {
	return start_line.version + ' ' + start_line.status + "\r\n";
}