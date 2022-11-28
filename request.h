#pragma once

#include <sstream>

#include "baserr.h"
#include "status_code.h"

class Request : public BaseRR {
private:
	std::string data_format;
private:
	void readStartLine(std::stringstream&);
	void readHeaders(std::stringstream&);
	void readBody(std::stringstream&);
public:
	Request(std::stringstream);
	virtual const std::string getStartLine() const;
	const std::string& getStatus() const = delete;
	const std::string& getDataFormat() const;
	const std::string& getHeaderValue(const std::string&) const;
};