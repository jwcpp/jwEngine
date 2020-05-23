#include "JsonReader.h"
#include "Json.h"
#include <cstring>

using namespace jwEngine;

bool JsonReader::readStr(std::string & outstr, JsonBuff * buff)
{
	const char * str = buff->curr();
	if (*str == '"')
	{
		const char * ps = buff->go();
		const char * p = ps;
		while (*p != '"' && *p != '\0')
		{
			p = buff->go();
		}

		if (ps != p && *p == '"')
		{
			buff->go();
			outstr = std::string(ps, p - ps);
			return true;
		}
	}

	return false;
}

JBaseObj * JsonReader::readBase(JsonBuff * buff)
{
	const char * str = buff->curr();
	if (!strncmp(str, "null", 4))
	{
		auto obj = new JValue();
		obj->setNull();
		buff->go(4);
		return obj;
	}

	if (!strncmp(str, "false", 5))
	{
		auto obj = new JValue();
		obj->setBool(false);
		buff->go(5);
		return obj;
	}

	if (!strncmp(str, "true", 4))
	{
		auto obj = new JValue();
		obj->setBool(true);
		buff->go(4);
		return obj;
	}

	// number
	bool isfloat = false;
	int numpos = 0;
	const char * pnum = str;
	
	if (*str == '-')
	{
		str = buff->go(1);
	}

	while (*str == '.' || (*str >= '0' && *str <= '9'))
	{
		if (*str == '.')
		{
			if (isfloat) return nullptr;
			isfloat = true;
		}
		else
		{
			numpos++;
		}
		str = buff->go(1);
	}

	if (numpos)
	{
		auto obj = new JValue();
		if (isfloat)
			obj->setFloat(std::stof(pnum));
		else
			obj->setInt(std::stoi(pnum));
		return obj;
	}

	// string
	std::string s;
	if (readStr(s, buff))
	{
		auto obj = new JValue();
		obj->setStr(s.c_str());
		return obj;
	}

	return nullptr;
}


JBaseObj * JsonReader::readArray(JsonBuff * buff)
{
	/*
	[1,2,[33,44],{"num": 666}]
	*/
	JArray * jarr = NULL;
	const char * pstr = buff->curr();
	std::vector<JBaseObj *> objs;
	while (*pstr != ']' && *pstr != '\0')
	{
parser_pos:
		JBaseObj * obj = reader(buff);
		if(!obj)
			goto fail;

		objs.push_back(obj);

		if (*(pstr = buff->skip()) == ',')
		{
			buff->go();
			goto parser_pos;
		}
	}

	if (*pstr == ']')
		buff->go();
	else
		goto fail;

	jarr = new JArray;
	for (auto & value : objs)
	{
		jarr->items.push_back(value);
	}
	return jarr;

fail:
	for (auto & value : objs)
	{
		delete value;
	}
	return nullptr;
}

JBaseObj * JsonReader::readObj(JsonBuff * buff)
{
	/*
	{
		"a": 10,
		"b": 20
	}
	*/
	JSonObj * jobj = NULL;
	const char * pstr = buff->curr();
	std::map<std::string, JBaseObj *> objs;
	while (*pstr != '}' && *pstr != '\0')
	{
	parser_pos:
		pstr = buff->skip();
		if (*pstr == '"')
		{
			// "key"
			std::string s;
			if (!readStr(s, buff))
				goto fail;

			// "key" :
			pstr = buff->curr();
			if (*pstr != ':')
				goto fail;
			pstr = buff->go();

			// "key" : value
			JBaseObj * obj = reader(buff);
			if (!obj)
				goto fail;

			objs[s] = obj;

			if (*(pstr = buff->skip()) == ',')
			{
				pstr = buff->go();
				goto parser_pos;
			}
		}
		else
		{
			goto fail;
		}
	}

	if (*pstr == '}')
		buff->go();
	else
		goto fail;

	jobj = new JSonObj;
	for (auto &[key, value] : objs)
	{
		jobj->maps[key] = value;
	}
	return jobj;

fail:
	for (auto &[key, value] : objs)
	{
		delete value;
	}
	return nullptr;
}

JBaseObj * JsonReader::reader(JsonBuff * buff)
{
	const char * pstr = buff->skip();
	switch (*pstr)
	{
	case '[':
		buff->go();
		return readArray(buff);
		break;
	case '{':
		buff->go();
		return readObj(buff);
		break;
	default:
		return readBase(buff);
		break;
	}

	return nullptr;
}

JBaseObj * JsonReader::parser(JsonBuff * buff)
{
	JBaseObj * obj = reader(buff);
	const char *p = buff->skip();
	if(obj)
	{
		if (*p != '\0')
		{
			delete obj;
			return nullptr;
		}

		buff->obj = obj;
	}
	return obj;
}

std::string JsonReader::getError(JsonBuff * buff)
{
	std::string err;
	const char *p = buff->curr();
	if (*p != '\0')
	{
		err = std::string("There is an anomaly near '")+(*p) + "'";
	}
	else
	{
		err = "String ends prematurely";
	}

	return err;
}

std::string JsonReader::write(JBaseObj * obj, bool layout)
{
	std::string str;
	obj->write(str, layout);
	return str;
}