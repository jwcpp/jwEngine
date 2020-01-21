
extern "C" {
#include "lua/lua.h"  
#include "lua/lauxlib.h"  
#include "lua/lualib.h"  
}

#include <stdio.h>
#include <stdlib.h>

//------------------type
#include "stdint.h"

typedef int8_t int8;
typedef uint8_t uint8;

typedef int16_t int16;
typedef uint16_t uint16;

typedef int32_t int32;
typedef uint32_t uint32;

typedef int64_t int64;
typedef uint64_t uint64;
//----------------------
#include <functional>

class Test
{
public:
	Test()
	{
	
	}

	~Test()
	{
		printf("~Test\n");
	}

	void setId(uint32 id_){ id = id_; }
	uint32 getId(){ return id; }

	uint32 id;
};


class GO
{
public:

	Test * create(){ return new Test(); }


	Test* run()
	{
		Test * t = create();
		t->id = 666;
		return t;
	}

	void backfunc(std::function<void(int)> cb)
	{
		cb(123);
	}
};

#include "sol/sol.hpp"

void func(sol::table t)
{
	int a = t["a"];
	int b = t["b"];

	printf("%d-%d\n", a, b);
	printf("....");
}

int main()
{
	sol::state lua;
	lua.open_libraries();


	lua.new_usertype<Test>("Test",
		"setId", &Test::setId,
		"getId", &Test::getId);

	lua.new_usertype<GO>("Go",
		"run", &GO::run,
		"backfunc", &GO::backfunc);

	lua.set_function("func", &func);

	lua.script_file("a.lua");



	system("pause");
	return 0;
}