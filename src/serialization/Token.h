/************************************************************************
* @file      Token.h
* @brief     ½âÎöÁîÅÆµÈ¹Ø¼ü×Ö
* @author    jiangwang
* @data      2019-12-25
* @version   0.1
************************************************************************/

#pragma once

#include <string>

class FBuffer;
class Token
{
public:
	Token(FBuffer * buffer);
	~Token();

	int nextToken();
	const std::string & getName() const { return __m_name; }
	int getInt(){ return __m_i; }
	float getFloat(){ return __m_f; }
	int getToken(){ return __m_token; }
	FBuffer * getBuffer(){ return __m_pBuffer; }
private:
	char next();
	bool check_next(char c);
	int read_num();
	int read_var();
	int symbol();
	void read_long_string();
private:
	FBuffer * __m_pBuffer;
	std::string __m_name;
	int __m_i;
	float __m_f;
	int __m_token;
};

