#include "sol/sol.hpp"
#include "UTimer.h"
#include "XLog.h"
#include "Tools.h"

class Lua_XLog
{
public:
	static void info_log(const char * log) { INFO_LOG(log); }
	static void warning_log(const char * log) { WARNING_LOG(log); }
	static void error_log(const char * log) { ERROR_LOG(log); }
};

void luabind_common(sol::state & lua)
{
	// log
	lua["infoLog"] = &Lua_XLog::info_log;
	lua["warningLog"] = &Lua_XLog::warning_log;
	lua["errorLog"] = &Lua_XLog::error_log;

	// timer
	lua.new_usertype<UTimer>("UTimer",
		"start", &UTimer::start,
		"stop", &UTimer::stop);

	lua["sleep"] = &Tools::sleep;
}