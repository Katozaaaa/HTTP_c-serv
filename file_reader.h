#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include "status_code.h"

// ENTER YOUR ROOT PATH
#define ROOT_PATH "C:/Users/hjvfn/Desktop/katoza-site/"

std::ifstream* openFile(const std::string&);
std::string readFile(std::ifstream*, const std::string);