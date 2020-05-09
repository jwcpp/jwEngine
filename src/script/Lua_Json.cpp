#include "sol/sol.hpp"
#include "Json.h"
#include "JsonReader.h"

using namespace jwEngine;

JValue * json_to_value(JBaseObj * obj)
{
	return static_cast<JValue *>(obj);
}

JArray * json_to_array(JBaseObj * obj)
{
	return static_cast<JArray *>(obj);
}

JSonObj * json_to_obj(JBaseObj * obj)
{
	return static_cast<JSonObj *>(obj);
}

void luabind_json(sol::state & lua)
{

	lua["json_to_value"] = &json_to_value;
	lua["json_to_array"] = &json_to_array;
	lua["json_to_obj"] = &json_to_obj;
	lua["json_parser"] = &JsonReader::parser;
	lua["json_error"] = &JsonReader::getError;
	lua["json_write"] = &JsonReader::write;

	lua.new_usertype<JBaseObj>("JBaseObj",
		"type", &JBaseObj::type);

	lua.new_usertype<JValue>("JValue",
		sol::base_classes, sol::bases<JBaseObj>(),
		"setInt", &JValue::setInt,
		"setFloat", &JValue::setFloat,
		"setStr", &JValue::setStr,
		"setBool", &JValue::setBool,
		"setNull", &JValue::setNull);

	lua.new_usertype<JArray>("JArray",
		sol::base_classes, sol::bases<JBaseObj>(),
		"pushInt", &JArray::pushInt,
		"pushFloat", &JArray::pushFloat,
		"pushStr", &JArray::pushStr,
		"pushBool", &JArray::pushBool,
		"pushNull", &JArray::pushNull,
		
		"getInt", &JArray::getInt,
		"getFloat", &JArray::getFloat,
		"getStr", &JArray::getStr,
		"getBool", &JArray::getBool,
		"isNull", &JArray::isNull,

		"setInt", &JArray::setInt,
		"setFloat", &JArray::setFloat,
		"setStr", &JArray::setStr,
		"setBool", &JArray::setBool,
		"setNull", &JArray::setNull,

		"createArray", &JArray::createArray,
		"createObj", &JArray::createObj,
		"getJsonObj", &JArray::getJsonObj,

		"remove", &JArray::remove,
		"size", &JArray::size);

	lua.new_usertype<JSonObj>("JSonObj",
		sol::base_classes, sol::bases<JBaseObj>(),
		"getInt", &JSonObj::getInt,
		"getFloat", &JSonObj::getFloat,
		"getStr", &JSonObj::getStr,
		"getBool", &JSonObj::getBool,
		"isNull", &JSonObj::isNull,

		"setInt", &JSonObj::setInt,
		"setFloat", &JSonObj::setFloat,
		"setStr", &JSonObj::setStr,
		"setBool", &JSonObj::setBool,
		"setNull", &JSonObj::setNull,

		"createArray", &JSonObj::createArray,
		"createObj", &JSonObj::createObj,
		"getJsonObj", &JSonObj::getJsonObj,

		"remove", &JSonObj::remove);


	lua.new_usertype <JsonBuff>("JsonBuff",
		sol::constructors<JsonBuff(const char *)>());
}