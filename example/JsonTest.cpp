#include <stdio.h>
#include <map>
#include "Json.h"

using namespace jwEngine;

// obj
int main_1()
{
	std::string str;
	{
		JSonObj obj;
		obj.setInt("num", 10);

		// array
		JArray * j_arr = obj.createArray("arr");
		j_arr->pushInt(12);
		j_arr->pushInt(13);

		JArray * array1 = j_arr->createArray();
		array1->pushInt(33);
		array1->pushInt(44);

		JSonObj * obj1 = j_arr->createObj();
		obj1->setInt("count", 88);

		JArray * array2 = obj1->createArray("arr2");
		array2->pushInt(123);
		array2->pushInt(321);

		// obj
		JSonObj * obj2 = obj.createObj("obj");
		obj2->setInt("num", 1);
		obj2->setInt("num2",2);

		JSonObj * obj3 = obj2->createObj("obj2");
		obj3->setStr("str", "hello world");
		obj3->setBool("boo", false);

		JArray * array3 = obj3->createArray("array");
		array3->pushBool(true);
		array3->pushBool(false);

		obj.write(str);
	}

	printf("%s\n", str.c_str());

	return 0;
}

// array
int main_2()
{
	JArray arr;
	arr.pushInt(10);
	arr.pushInt(20);

	JSonObj * obj = arr.createObj();
	obj->setInt("num", 6);
	obj->setInt("count", 0);

	arr.pushBool(false);

	std::string s;
	arr.write(s);
	printf("%s\n", s.c_str());
	return 0;
}

// base value
int main_3()
{
	JValue value;
	value.setInt(10);
	printf("%d\n", value.iv);

	value.setStr("hello world");
	printf("%s\n", value.sv.c_str());
	return 0;
}

#include "JsonReader.h"
#include <iostream>
int main()
{
	//const char * p = "{\"num\": 11, \"obj\":{\"a\":12, \"b\":13}, \"array\":[1314,520]}";
	const char * p = "[\"haha\", false, null, 20.9, {\"num\":123, \"array\":[1,2,3,4]}]";
	JsonBuff buff(p);

	JBaseObj * jobj = JsonReader::parser(&buff);
	if (jobj)
	{
		std::string str = JsonReader::write(jobj);
		std::cout << str << std::endl;
	}
	else
	{
		std::cout <<"parser error:" << JsonReader::getError(&buff) << std::endl;
	}

	return 0;
}