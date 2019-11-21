
#ifndef XTIME_H
#define XTIME_H

#include <time.h>
#include <string>
#include "Platform.h"
#include "BaseType.h"

#ifdef SYSTEM_WIN
#include <Windows.h>
#else
#include <sys/time.h>
#endif // PLATFORM_WINDOWS


class XTime
{
public:
	/** @brief 判断是否是闰年 */
	static bool isLeapYear(int year);
	/** @brief 某年某月一共有多少天 */
	static int yearMonthDays(int year, int month);

	static const struct tm * getTMStruct();
	static time_t getTime(struct tm * tm_);
	static std::string format(const char * fmt = "%Y-%m-%d %H:%M:%S");

	static void getTimeval(struct timeval * tp);
	/** @brief 获取毫秒时间戳 */
	static int64 milliStamp();

	/** @brief 获取微秒时间戳 */
	static int64 microStamp();

	/** @brief 获取时间戳 */
	static time_t stamp();

	static void updateTime(time_t t = time(NULL));
private:
	static time_t _currt;
};

#endif