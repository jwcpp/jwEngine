#include "Json.h"

using namespace jwEngine;

void writeDepth(std::string & str, int depth)
{
	for (int i = 0; i < depth; ++i)
	{
		str.append("\t");
	}
}

JBaseObj::JBaseObj()
{

}
JBaseObj::~JBaseObj()
{

}

JValue::JValue()
{
	
}
JValue::~JValue()
{
	
}

void JValue::setInt(int v)
{
	type = eJ_INT;
	iv = v;
}
void JValue::setFloat(float v)
{
	type = eJ_FLOAT;
	fv = v;
}
void JValue::setStr(const char * v)
{
	type = eJ_STRING;
	sv = v;
}
void JValue::setBool(bool v)
{
	type = eJ_BOOL;
	bv = v;
}
void JValue::setNull()
{
	type = eJ_NULL;
}

void JValue::write(std::string & str, bool layout, int depth)
{
	switch (type)
	{
	case eJ_INT:
	{
		str.append(std::to_string(iv));
	}
	break;
	case eJ_FLOAT:
	{
		str.append(std::to_string(fv));
	}
	break;
	case eJ_BOOL:
	{
		if (bv)
		{
			str.append("true");
		}
		else
		{
			str.append("false");
		}
	}
	break;
	case eJ_NULL:
	{
		str.append("null");
	}
	break;
	case eJ_STRING:
	{
		str.append("\"" + sv + "\"");
	}
	break;
	default:
		break;
	}
}

JArray::JArray()
{
	type = (int)eJ_ARRAY;
}

JArray::~JArray()
{
	for (auto obj : items)
	{
		delete obj;
	}
}

void JArray::pushInt(int v)
{
	auto item = new JValue;
	item->setInt(v);
	items.push_back(item);
}
void JArray::pushFloat(float v)
{
	auto item = new JValue;
	item->setFloat(v);
	items.push_back(item);
}
void JArray::pushStr(const char * v)
{
	auto item = new JValue;
	item->setStr(v);
	items.push_back(item);
}
void JArray::pushBool(bool v)
{
	auto item = new JValue;
	item->setBool(v);
	items.push_back(item);
}
void JArray::pushNull()
{
	auto item = new JValue;
	item->setNull();
	items.push_back(item);
}

int JArray::getInt(int idx)
{
	return static_cast<JValue *>(items[idx])->iv;
}
float JArray::getFloat(int idx)
{
	return static_cast<JValue *>(items[idx])->fv;
}
bool JArray::getBool(int idx)
{
	return static_cast<JValue *>(items[idx])->bv;
}
const char * JArray::getStr(int idx)
{
	return static_cast<JValue *>(items[idx])->sv.c_str();
}

bool JArray::isNull(int idx)
{
	return items[idx]->type == eJ_NULL;
}

void JArray::setInt(int idx, int v)
{
	static_cast<JValue *>(items[idx])->setInt(v);
}
void JArray::setFloat(int idx, float v)
{
	static_cast<JValue *>(items[idx])->setFloat(v);
}
void JArray::setStr(int idx, const char * v)
{
	static_cast<JValue *>(items[idx])->setStr(v);
}
void JArray::setBool(int idx, bool v)
{
	static_cast<JValue *>(items[idx])->setBool(v);
}
void JArray::setNull(int idx)
{
	if (items[idx]->type == eJ_ARRAY || items[idx]->type == eJ_OBJ)
	{
		// need delete
		delete items[idx];

		items[idx] = new JValue;
	}
	static_cast<JValue *>(items[idx])->setNull();
}

void JArray::remove(int idx)
{
	auto it = items.begin() + idx;
	if (it != items.end())
	{
		delete *it;
		items.erase(it);
	}
}

int JArray::size()
{
	return items.size();
}
JArray * JArray::createArray()
{
	auto p = new JArray;
	items.push_back(p);
	return p;
}
JSonObj * JArray::createObj()
{
	auto p = new JSonObj;
	items.push_back(p);
	return p;
}

JBaseObj * JArray::getJsonObj(int idx)
{
	auto it = items.begin() + idx;
	return *it;
}

void JArray::write(std::string & str, bool layout, int depth)
{
	str.append("[");
	if (layout) str.append("\n");
	int count = 0, maxcount = items.size();;
	for (auto & obj : items)
	{
		if (layout) writeDepth(str, depth + 1);

		obj->write(str, layout, depth + 1);

		if (++count < maxcount) {
			str.append(",");
		}

		if (layout) str.append("\n");
	}

	if (layout) writeDepth(str, depth);
	str.append("]");
}

JSonObj::JSonObj()
{
	type = eJ_OBJ;
}

JSonObj::~JSonObj()
{
	for (auto &[key, value] : maps)
	{
		delete value;
	}
}

int JSonObj::getInt(std::string key)
{
	return static_cast<JValue *>(maps[key])->iv;
}
float JSonObj::getFloat(std::string key)
{
	return static_cast<JValue *>(maps[key])->fv;
}
bool JSonObj::getBool(std::string key)
{
	return static_cast<JValue *>(maps[key])->bv;
}
const char * JSonObj::getStr(std::string key)
{
	return static_cast<JValue *>(maps[key])->sv.c_str();
}
bool JSonObj::isNull(std::string key)
{
	return maps[key]->type == eJ_NULL;
}

void JSonObj::setInt(std::string key, int v)
{
	if (!maps[key])
	{
		maps[key] = new JValue;
	}
	static_cast<JValue *>(maps[key])->setInt(v);
}
void JSonObj::setFloat(std::string key, float v)
{
	if (!maps[key])
	{
		maps[key] = new JValue;
	}
	static_cast<JValue *>(maps[key])->setFloat(v);
}
void JSonObj::setStr(std::string key, const char * v)
{
	if (!maps[key])
	{
		maps[key] = new JValue;
	}
	static_cast<JValue *>(maps[key])->setStr(v);
}
void JSonObj::setBool(std::string key, bool v)
{
	if (!maps[key])
	{
		maps[key] = new JValue;
	}
	static_cast<JValue *>(maps[key])->setBool(v);
}

void JSonObj::setNull(std::string key)
{
	if (!maps[key])
	{
		maps[key] = new JValue;
	}
	else if (maps[key]->type == eJ_ARRAY || maps[key]->type == eJ_OBJ)
	{
		// need delete
		delete maps[key];

		maps[key] = new JValue;
	}
	static_cast<JValue *>(maps[key])->setNull();
}

void JSonObj::remove(std::string key)
{
	auto it = maps.find(key);
	if (it != maps.end())
	{
		delete it->second;
		maps.erase(it);
	}
}

JArray * JSonObj::createArray(std::string key)
{
	auto p = new JArray;
	maps[key] = p;
	return p;
}
JSonObj * JSonObj::createObj(std::string key)
{
	auto p = new JSonObj;
	maps[key] = p;
	return p;
}

JBaseObj * JSonObj::getJsonObj(std::string key)
{
	return maps[key];
}

void JSonObj::write(std::string & str, bool layout, int depth)
{
	str.append("{");
	if (layout) str.append("\n");
	int count = 0, maxcount = maps.size();
	for (auto &[key, obj] : maps)
	{
		if (layout) writeDepth(str, depth + 1);
		str.append("\"" + key + "\"" + ":");

		obj->write(str, layout, depth + 1);

		if (++count < maxcount)
		{
			str.append(",");
		}
		if (layout) str.append("\n");
	}

	if (layout) writeDepth(str, depth);
	str.append("}");
}