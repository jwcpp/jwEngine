#include "sol/sol.hpp"
#include "CsvParser.h"
#include <string>
#include "BaseType.h"

class Lua_CsvParser : public CsvParser
{
public:
	Lua_CsvParser(const char * file, int startline = 1, char sep = '\t'):
		CsvParser(file, startline, sep)
	{
	
	}

	//------->get
	int8 getInt8(int row, const char * name) { return getValue <int8>(row, name); }
	uint8 getUint8(int row, const char * name) { return getValue <uint8>(row, name); }
	int16 getInt16(int row, const char * name) { return getValue <int16>(row, name); }
	uint16 getUint16(int row, const char * name) { return getValue <uint16>(row, name); }
	int32 getInt32(int row, const char * name) { return getValue <int32>(row, name); }
	uint32 getUint32(int row, const char * name) { return getValue <uint32>(row, name); }
	int64 getInt64(int row, const char * name) { return getValue <int64>(row, name); }
	uint64 getUint64(int row, const char * name) { return getValue <uint64>(row, name); }
	float getFloat(int row, const char * name) { return getValue <float>(row, name); }
	double getDouble(int row, const char * name) { return getValue <double>(row, name); }
	std::string getString(int row, const char * name) { return getValue <std::string>(row, name); }
};

void luabind_csvpar(sol::state & lua)
{
	lua.new_usertype<Lua_CsvParser>("CsvParser",
		sol::constructors<Lua_CsvParser(const char *, int, char)>(),
		"getInt8", &Lua_CsvParser::getInt8,
		"getUint8", &Lua_CsvParser::getUint8,
		"getInt16", &Lua_CsvParser::getInt16,
		"getUint16", &Lua_CsvParser::getUint16,
		"getInt32", &Lua_CsvParser::getInt32,
		"getUint32", &Lua_CsvParser::getUint32,
		"getInt64", &Lua_CsvParser::getInt64,
		"getUint64", &Lua_CsvParser::getUint64,
		"getFloat", &Lua_CsvParser::getFloat,
		"getDouble", &Lua_CsvParser::getDouble,
		"getString", &Lua_CsvParser::getString,
		
		"row", &Lua_CsvParser::row);
}