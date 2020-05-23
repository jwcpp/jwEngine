#pragma once
/************************************************************************
* @file      HttpParam.h
* @brief     Parse HTTP parameters
* @author    jiangwang
* @data      2020-5-21
* @version   0.1
************************************************************************/

class HttpParam
{
public:
	HttpParam(std::string_view str_v, char cut = '&');

	static void splite(std::string_view str_view, std::vector<std::string_view> & vec_str, char cut = '&');

	bool find(std::string_view key);
	std::string_view getStr(std::string_view key);
	int getInt(std::string_view key);
	float getFloat(std::string_view key);

private:
	std::map<std::string_view, std::string_view> params;
};

