#pragma once

#include <string>
class Tools
{
public:
	static void encrypt(char * buf, char * key);
	static std::string format(const char * format, ...);
	static void mySleep(int millionseconds);
	static unsigned long getCurrentThreadID();
};

