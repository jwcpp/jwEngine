#include <string_view>
#include <vector>
#include <string>
#include <map>
#include "HttpParam.h"

HttpParam::HttpParam(std::string_view str_v, char cut)
{
	std::vector<std::string_view> equal;
	splite(str_v, equal, cut);
	
	for (auto it = equal.begin();it!= equal.end(); ++it)
	{
		std::string_view & sub = (*it);

		std::vector<std::string_view> kys;
		splite(sub, kys, '=');

		if (kys.size() == 2)
		{
			params[kys[0]] = kys[1];
		}
	}
}


void HttpParam::splite(std::string_view str_view, std::vector<std::string_view> & vec_str, char cut)
{
	std::string::size_type pos1 = 0, pos2;
	while ((pos2 = str_view.find(cut, pos1)) != std::string_view::npos)
	{
		vec_str.push_back(str_view.substr(pos1, pos2 - pos1));
		pos1 = pos2 + 1;
	}
	vec_str.push_back(str_view.substr(pos1));
}

bool HttpParam::find(std::string_view key)
{
	return params.find(key) != params.end();
}

std::string_view HttpParam::getStr(std::string_view key)
{
	return params[key];
}

int HttpParam::getInt(std::string_view key)
{
	std::string si(params[key]);
	return std::stoi(si);
}

float HttpParam::getFloat(std::string_view key)
{
	std::string sf(params[key]);
	return std::stof(sf);
}