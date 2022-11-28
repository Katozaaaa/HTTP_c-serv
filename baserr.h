#pragma once

#include <string>
#include <map>

struct StartLine {
	std::string method;
	std::string URL;
	std::string version;
	std::string status;
};

class BaseRR {
protected:
	StartLine start_line;
	std::map <std::string, std::string> headers;
	std::string body;
public:
	virtual const std::string getStartLine() const = 0;
	const std::string getAll() const;
	const std::string getHeaders() const;
	const std::string getBody() const;
	const std::string& getMethod() const;
	const std::string& getURL() const;
	const std::string& getVersion() const;
	const std::string& getStatus() const;
};