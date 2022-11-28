#include "baserr.h"
#include <sstream>

const std::string BaseRR::getAll() const {
	return getStartLine() + getHeaders() + getBody();
}
const std::string BaseRR::getHeaders() const {
	std::stringstream headers_str;
	for (auto header : headers) {
		headers_str << header.first << ": " << header.second << "\r\n";
	}
	headers_str << "\r\n";
	return headers_str.str();
}
const std::string BaseRR::getBody() const {
	return body + "\r\n";
}
const std::string& BaseRR::getMethod() const {
	return start_line.method;
}
const std::string& BaseRR::getURL() const {
	return start_line.URL;
}
const std::string& BaseRR::getVersion() const {
	return start_line.version;
}
const std::string& BaseRR::getStatus() const {
	return start_line.status;
}
