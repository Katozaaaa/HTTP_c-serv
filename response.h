#pragma once

#include <sstream>

#include "baserr.h"
#include "status_code.h"
#include "file_reader.h"
#include "request.h"

class Response : public BaseRR {
public:
	Response(const Request&);
	virtual const std::string getStartLine() const;
	const std::string& getMethod() const = delete;
	const std::string& getURL() const = delete;
};