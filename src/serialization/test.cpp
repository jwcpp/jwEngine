
#include <stdio.h>
#include <stdlib.h>
#include "Token.h"
#include "Fio.h"
#include <assert.h>
#include "Parser.h"
#include "Event.h"
#include "Typedef.h"

class TEvent : public Event
{
public:
	virtual void onTypedef(TypeInfo * tinfo, const char * name)
	{
		printf("%d|%d|%s|%s\n", tinfo->type, tinfo->value, tinfo->name.c_str(), name);
	}
	// 定义变量
	virtual void onVariable(TypeInfo * tinfo, const char * varName, const char * exp)
	{
		printf("var： %d|%d|%s|%s = %s\n", tinfo->type, tinfo->value, tinfo->name.c_str(), varName, exp);
	}

	virtual void onEnterStruct(const char * name)
	{
		printf("enter struct %s\n", name);
	}
	virtual void onExitStruct()
	{
		printf("exit struct\n");
	}

	virtual void onEnterRead(){
		printf("onEnterRead \n");
	}
	virtual void onExitRead() {
		printf("onExitRead \n");
	}
	virtual void onEnterWrite() {
		printf("onEnterWrite \n");
	}
	virtual void onExitWrite() {
		printf("onExitWrite \n");
	}
	virtual void onReadWriteVar(TypeInfo * tinfo, const char * varName)
	{
		printf("read write： %d|%d|%s|%s \n", tinfo->type, tinfo->value, tinfo->name.c_str(), varName);
	}
	virtual void onValue(const char * varName, const char * exp)
	{
		printf("%s = %s \n", varName, exp);
	}

	virtual void onEnterIf()
	{
		printf("if ");
	}
	virtual void onElseIf()
	{
		printf("}else if");
	}

	virtual void onIfCond(const char * condition)
	{
		printf("(%s) { \n", condition);
	}

	virtual void onElse()
	{
		printf("}else { \n");
	}
	virtual void onExitIf()
	{
		printf("} \n");
	}

	virtual void onImport(const char * filename)
	{
		printf("import %s \n",filename);
	}

};

int test_main()
{
	FBuffer fbuff;
	if (!fbuff.load("test.txt"))
	{
		assert(0);
	}

	Token token(&fbuff);

	TEvent tev;
	Parser parser(&token, &tev);
	parser.start();
	
	system("pause");
	return 0;
}
