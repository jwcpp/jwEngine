#include "sol/sol.hpp"
#include "UTimer.h"
#include "XLog.h"
#include "Tools.h"
#include "XFile.h"
#include "XTime.h"

class Lua_XLog
{
public:
	static void info_log(const char * log) { INFO_LOG(log); }
	static void warning_log(const char * log) { WARNING_LOG(log); }
	static void error_log(const char * log) { ERROR_LOG(log); }
};

void luabind_common(sol::state & lua)
{
	// file
	lua["writeFile"] = &XFile::writeFile;
	lua["isExist"] = &XFile::isExist;
	lua["mkdir"] = &XFile::mkdir;
	lua["rmdir"] = &XFile::rmdir;
	lua["createDir"] = &XFile::createDirectory;

	// log
	lua["infoLog"] = &Lua_XLog::info_log;
	lua["warningLog"] = &Lua_XLog::warning_log;
	lua["errorLog"] = &Lua_XLog::error_log;

	// timer
	lua.new_usertype<UTimer>("UTimer",
		"start", &UTimer::start,
		"stop", &UTimer::stop);

	// tool
	lua["sleep"] = &Tools::sleep;
	lua["gbk_utf8"] = &Tools::gbkToUtf8;
	lua["utf8_gbk"] = &Tools::utf8ToGbk;
	lua["srand"] = &Tools::srand;
	lua["random"] = &Tools::random;

	// time
	lua["isLeapYear"] = &XTime::isLeapYear;
	lua["yearMonthDays"] = &XTime::yearMonthDays;
	lua["milliStamp"] = &XTime::milliStamp;
	lua["microStamp"] = &XTime::microStamp;
	lua["stamp"] = &XTime::stamp;
	lua["iclock"] = &XTime::iclock;
}