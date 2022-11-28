cmake_minimum_required(VERSION 3.24)

project(http_server)

set(SOURCES main.cpp server.cpp baserr.cpp request.cpp response.cpp file_reader.cpp)
set(HEADERS server.h baserr.h request.h response.h file_reader.h status_code.h print_result.h)

add_executable(http_server ${SOURCES} ${HEADERS})
set_target_properties (
	http_server PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)
