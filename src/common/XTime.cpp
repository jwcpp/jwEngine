#include "XTime.h"
#include <limits.h>
#include <sstream>
#include <iomanip>
#include <string.h>
using namespace std;

#ifdef SYSTEM_WIN

int gettimeofday(struct timeval *tp, void *tzp) 
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = static_cast<long>(clock);
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#endif // PLATFORM_WINDOWS


bool XTime::isLeapYear(int year)
{
	return (year % 4 == 0 && ((year % 400 == 0) || (year % 100 != 0)));
}

int XTime::yearMonthDays(int year, int month)
{
	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		//Need to consider a leap year in February
		return isLeapYear(year) ? 29 : 28;
	default:
		return 0;
	}
}


const struct tm * XTime::getTMStruct()
{
	time_t _currt = time(NULL);
	return std::localtime(&_currt);
}

std::string XTime::format(const char * fmt)
{
	char buf[256] = { 0 };
	if (0 == strftime(buf, 63, fmt, getTMStruct())) {
		buf[0] = '\0';
	}
	return std::string(buf);
}

time_t XTime::getTime(struct tm * tm_)
{
	return std::mktime(tm_);
}

void XTime::getTimeval(struct timeval * tp)
{
	gettimeofday(tp, NULL);
}

// msec
int64 XTime::milliStamp()
{
	struct timeval tv;
	getTimeval(&tv);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

// usec
int64 XTime::microStamp()
{
	struct timeval tv;
	getTimeval(&tv);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

// sec
time_t XTime::stamp()
{
	struct timeval tv;
	getTimeval(&tv);
	return tv.tv_sec;
}

uint32 XTime::iclock()
{
	return (uint32)((double)std::clock() / CLOCKS_PER_SEC * 1000);
}