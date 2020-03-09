#pragma once

#include <map>
#include <set>

class TypeInfo
{
public:
	// 0: base 1:vector
	int type = 0;

	//base
	int value;
	std::string name;
	
	// vector  [typedef vector<int> VEC]
	std::string vec_name;
};


class Typedef
{
public:

	static void addStruct(const char * key);
	static bool isStruct(const char * name);

	static void addTypeInfo(const char * name, TypeInfo & info);
	static TypeInfo * getTypeInfo(const char * name);
	static bool isVector(const char * name);
private:
	static std::map<std::string, TypeInfo> typedef_; // typedef int32 ID
	static std::set<std::string> custom_type;		  // struct name
};

