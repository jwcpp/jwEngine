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


void Tools::sleep(int ms)
{
#if defined(SYSTEM_WIN)
	Sleep(ms);
#else
	::usleep(ms * 1000);
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


int Tools::binarySearch(int arr[], int len, int findX)
{
	int left = 0; int right = len - 1;
	while (left <= right) {
		int middle = (left + right) / 2;
		if (findX == arr[middle]) return middle;
		if (findX > arr[middle])left = middle + 1;
		else right = middle - 1;
	}

	return -1;
}

#ifdef SYSTEM_LINUX

#include <iconv.h>

static int iconv_convert(const char *src_str, size_t src_len, char *dst_str, size_t dst_len, const char * from_charset = "gbk", const char * to_charset = "utf8")
{
	iconv_t cd;
	char **pin = (char **)&src_str;
	char **pout = &dst_str;

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
		return -1;
	memset(dst_str, 0, dst_len);
	if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
		return -1;
	iconv_close(cd);

	return 0;
}

#endif

std::string Tools::gbkToUtf8(const char *src_str)
{
	std::string strTemp;
#ifdef SYSTEM_WIN
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;

#else
	char dst_gbk[1024] = { 0 };
	iconv_convert(src_str, strlen(src_str), dst_gbk, sizeof(dst_gbk), "gbk", "utf8");
	strTemp = dst_gbk;

#endif

	return strTemp;
}

std::string Tools::utf8ToGbk(const char *src_str)
{
	std::string strTemp;
#ifdef SYSTEM_WIN
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	strTemp = szGBK;
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;

#else
	char dst_gbk[1024] = { 0 };
	iconv_convert(src_str, strlen(src_str), dst_gbk, sizeof(dst_gbk), "utf8", "gbk");
	strTemp = dst_gbk;

#endif
	return strTemp;
}

int Tools::charCount(char * pStr, char c)
{
	int nCount = 0;

	char* pTemp = pStr;
	while (*pTemp != '\0')
	{
		if (*pTemp == c)
		{
			nCount += 1;
		}

		pTemp += 1;
	}

	return nCount;
}

void Tools::srand(unsigned int t)
{
	::srand(t);
}

int Tools::random(int begin, int end)
{
	int value;
#ifdef SYSTEM_WIN
	value = rand() % (end - begin + 1);
#else
	value = ::random() % (end - begin + 1);
#endif

	return value + begin;
}