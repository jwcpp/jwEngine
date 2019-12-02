#include "Tools.h"
#include <string.h>
#include <stdarg.h>
#include "Define.h"


void Tools::encrypt(char * buf, char * key)
{
	char * p = buf;
	char * pKey = key;

	unsigned int bufLen = strlen(buf);
	unsigned int keyLen = strlen(pKey);

	for (unsigned int i = 0; i < bufLen; i++)
	{
		*p ^= pKey[i % keyLen];
		p++;
	}
}

std::string Tools::format(const char * format, ...)
{
	va_list vl;
	va_start(vl, format);

	int len = vsnprintf(NULL, 0, format, vl);
	std::string s("", len + 1);
	char * buff = (char *)s.data();
	vsprintf(buff, format, vl);

	va_end(vl);

	return s;
}


void Tools::mySleep(int millionseconds)
{
#if defined(SYSTEM_WIN)
	Sleep(millionseconds);
#else
	usleep(millionseconds * 1000);
#endif
}

unsigned long Tools::getCurrentThreadID()
{
#if defined(SYSTEM_WIN)
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}