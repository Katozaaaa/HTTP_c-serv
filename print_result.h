#pragma once

#include <iostream>
#include <string>
#include <windows.h>

#define SUCCESS "success"
// text color contains yellow.
#define FOREGROUND_YELLOW 0x0006
// text color contains dafault white.
#define FOREGROUND_DEFAULT 0x0007

template <typename T>
void print_result(const WORD& color, const T& result) {
	static const HANDLE& hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	std::cerr << result << '\n';
	SetConsoleTextAttribute(hConsole, FOREGROUND_DEFAULT);
}