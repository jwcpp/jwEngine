extern "C" {
#include "lua/lua.h"  
#include "lua/lauxlib.h"  
#include "lua/lualib.h"  
}

#include <stdio.h>
#include <stdlib.h>

#include "sol/sol.hpp"

extern void luabind_netpacket(sol::state & lua);


int main()
{
	sol::state lua;
	lua.open_libraries();

	// bind c++ && lua
	luabind_netpacket(lua);


	lua.script_file("main.lua");

	system("pause");
	return 0;
}