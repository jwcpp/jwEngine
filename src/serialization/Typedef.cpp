#include <string>
#include "Typedef.h"

std::map<std::string, TypeInfo> Typedef::typedef_; // typedef int32 ID
std::set<std::string> Typedef::custom_type;		  // struct name


void Typedef::addTypeInfo(const char * name, TypeInfo & info)
{
	typedef_[name] = info;
}

TypeInfo * Typedef::getTypeInfo(const char * name)
{
	auto it = typedef_.find(name);
	if (it != typedef_.end())
	{
		return &(it->second);
	}

	return NULL;
}

bool Typedef::isVector(const char * name)
{
	TypeInfo * info = getTypeInfo(name);
	if (!info)
		return false;

	if (info->type == 0)
		return false;

	return true;
}

bool Typedef::isStruct(const char * name)
{
	return custom_type.find(name) != custom_type.end();
}

void Typedef::addStruct(const char * key)
{
	custom_type.insert(std::set<std::string>::value_type(key));
}