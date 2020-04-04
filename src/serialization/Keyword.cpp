#include "Keyword.h"


std::map<std::string, int> Keyword::__m_keywords = {
	{ "if", eKw_IF },
	{ "else", eKw_ELSE },
	{ "struct", eKw_STRUCT },
	{ "typedef", eKw_TYPEDEF},
	{ "vector", eKw_VECTOR },
	{ "read", eKw_READ },
	{ "write", eKw_WRITE },
	{ "import", eKw_IMPORT },
	{ "int8", eKw_INT8 },
	{ "uint8", eKw_UINT8 },
	{ "int16", eKw_INT16 },
	{ "uint16", eKw_UINT16 },
	{ "int32", eKw_INT32 },
	{ "uint32", eKw_UINT32 },
	{ "int64", eKw_INT64 },
	{ "uint64", eKw_UINT64 },
	{ "float", eKw_FLOAT },
	{ "double", eKw_DOUBLE },
	{ "string", eKw_STRING },
};

Keyword::Keyword()
{
}


Keyword::~Keyword()
{
}

int Keyword::getKeyword(const char * name)
{
	auto it = __m_keywords.find(name);
	if (it == __m_keywords.end())
		return 0;
	return it->second;
}

bool Keyword::isBaseType(int value)
{
	switch (value)
	{
	case eKw_INT8:
	case eKw_UINT8:
	case eKw_INT16:
	case eKw_UINT16:
	case eKw_INT32:
	case eKw_UINT32:
	case eKw_INT64:
	case eKw_UINT64:
	case eKw_FLOAT:
	case eKw_DOUBLE:
		return true;

	default:
		return false;

	}
}

bool Keyword::isStrType(int value)
{
	return value == eKw_STRING;
}