#include "request.h"

void Request::readStartLine(std::stringstream& stream) {
	std::string start_line_str;
	getline(stream, start_line_str);
	if (*(start_line_str.end() - 1) == '\r') {
		start_line_str.erase((start_line_str.end() - 1), start_line_str.end());
	}
	else throw BAD_REQUEST;

	std::stringstream start_line_stream(start_line_str);
	if (!start_line_stream.eof()) {
		start_line_stream >> start_line.method;
		if (start_line.method != "GET") throw NOT_IMPLEMENTED;
	} 
	else throw BAD_REQUEST;

	if (!start_line_stream.eof()) {
		start_line_stream >> start_line.URL;
		if (start_line.URL.find("..") != std::string::npos) throw FORBIDDEN;
	}
	else throw BAD_REQUEST;

	if (!start_line_stream.eof()) {
		start_line_stream >> start_line.version;
		if (start_line.version.find("HTTP/") == std::string::npos) throw BAD_REQUEST;
		else if (start_line.version != "HTTP/1.1") throw HTTP_NOT_SUPPORTED;
	}
	else throw BAD_REQUEST;
}

void Request::readHeaders(std::stringstream& stream) {
	while (!stream.eof()) {
		std::string header;
		getline(stream, header);
		if (!header.empty()) {
			if (*(header.end() - 1) == '\r') {
				header.erase((header.end() - 1), header.end());
			}
			else throw BAD_REQUEST;
		}
		else break;

		std::string header_key, header_value;
		if (!header.empty()) {
			for (char sym : header) {
				if (sym == ':' && header_key.empty()) {
					header_key = header_value;
					header_value.erase();
				}
				else if (sym == ' ' && header_value.empty()) {
					continue;
				}
				else {
					header_value.push_back(sym);
				}
			}
			if (!header_key.empty() && !header_value.empty())
				headers[header_key] = header_value;
		}
	}
}
void Request::readBody(std::stringstream& stream) {
	while (!stream.eof()) {
		std::string str;
		getline(stream, str);
		body += str + '\n';
	}
}

Request::Request(std::stringstream stream) {
	readStartLine(stream);
	readHeaders(stream);
	if (headers.size() == 0) throw BAD_REQUEST;
	readBody(stream);

	if (*(--start_line.URL.end()) != '/') {
		if (start_line.URL.find(".") == std::string::npos) throw BAD_REQUEST;
		for (auto it = --start_line.URL.end(); *it != '.'; --it) {
			data_format += *it;
		}
		std::reverse(data_format.begin(), data_format.end());
	}
	else {
		data_format = "html";
	}
}
const std::string Request::getStartLine() const {
	return start_line.method + ' ' + start_line.URL + ' ' + start_line.version + "\r\n";
}
const std::string& Request::getDataFormat() const {
	return data_format;
}
const std::string& Request::getHeaderValue(const std::string& header_key) const {
	if (headers.find(header_key) != headers.end()) return headers.find(header_key)->second;
	else throw BAD_REQUEST;
}