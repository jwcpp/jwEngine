extern "C" {
#include "lua/lua.h"  
#include "lua/lauxlib.h"  
#include "lua/lualib.h"  
}

#include <stdio.h>
#include <stdlib.h>

#include "sol/sol.hpp"

extern void luabind_netserver(sol::state & lua);
extern void luabind_eventloop(sol::state & lua);
extern void luabind_mysql(sol::state & lua);
extern void luabind_common(sol::state & lua);
extern void luabind_redis(sol::state & lua);

int main(int argc, char* argv[])
{
	sol::state lua;
	lua.open_libraries();

	// bind c++ && lua
	luabind_netserver(lua);
	luabind_eventloop(lua);
	luabind_mysql(lua);
	luabind_common(lua);
	luabind_redis(lua);

	lua.script_file(argv[1]);

	system("pause");
	return 0;
}