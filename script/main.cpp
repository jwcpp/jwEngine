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


class A
{
public:
	A()
	{
		go();
	}

	virtual void go() {
		printf("A:go\n");
	}
};


int main(int argc, char* argv[])
{
	sol::state lua;
	lua.open_libraries();

	// bind c++ && lua
	luabind_netserver(lua);
	luabind_eventloop(lua);

	lua.script_file(argv[1]);

	system("pause");
	return 0;
}