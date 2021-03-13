#pragma once

#include <string>
class Tools
{
public:
	static void encrypt(char * buf, char * key);
	static std::string format(const char * format, ...);
	static void sleep(int ms);
	static unsigned long getCurrentThreadID();
	static int binarySearch(int arr[], int len, int findX);
	static std::string gbkToUtf8(const char *src_str);
	static std::string utf8ToGbk(const char *src_str);
	static int charCount(char * pStr, char c);
	static void srand(unsigned int);
	static int random(int begin, int end);
};

