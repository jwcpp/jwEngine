/************************************************************************
* @file      Keyword.h
* @brief     关键字列表
* @author    jiangwang
* @data      2019-12-26
* @version   0.1
************************************************************************/
#pragma once

enum KeyEnum
{
	eKw_EOF = 0,
	eKw_NUM = 128,
	eKw_DECIMALS,
	eKw_INT8,
	eKw_UINT8,
	eKw_INT16,
	eKw_UINT16,
	eKw_INT32,
	eKw_UINT32,
	eKw_INT64,
	eKw_UINT64,
	eKw_FLOAT,
	eKw_DOUBLE,
	eKw_STRING,

	eKw_READ,
	eKw_WRITE,
	eKw_STRUCT,
	eKw_TYPEDEF,
	eKw_IF,
	eKw_ELSE,
	eKw_VECTOR,
	eKw_IMPORT,
	
	eKw_EQ, // ==
	eKw_NE,	// !=
	eKw_AND,// &&
	eKw_OR, // ||
	eKw_LE, // <=
	eKw_GE, // >=
	eKw_SHL, // <<
	eKw_SHR, // >>

	eKw_VAR,
};

// 跨平台不允许：++ -- += -= *= /=


#include <string>
#include <map>
class Keyword
{
public:
	Keyword();
	~Keyword();

	static int getKeyword(const char * name);
	static bool isBaseType(int value);
	static bool isStrType(int value);

private:
	static std::map<std::string, int> __m_keywords;
};

