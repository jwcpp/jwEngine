#pragma once

/************************************************************************
* @file      GenerateLua.h
* @brief     Éú³Élua´úÂë
* @author    jiangwang
* @data      2019-3-3
* @version   0.1
************************************************************************/

#include "Event.h"
#include <string>
#include <vector>
class GenerateLua : public Event
{
public:
	void init();
	std::string getfile() { return __m_file; }
protected:
	virtual void onTypedef(TypeInfo * tinfo, const char * name);

	virtual void onEnterStruct(const char * name);
	virtual void onExitStruct();
	virtual void onVariable(TypeInfo * tinfo, const char * varName, const char * exp);

	virtual void onEnterRead();
	virtual void onExitRead();
	virtual void onEnterWrite();
	virtual void onExitWrite();
	virtual void onValue(const char * varName, const char * exp);
	virtual void onReadWriteVar(TypeInfo * tinfo, const char * varName);

	virtual void onEnterIf();
	virtual void onElseIf();
	virtual void onIfCond(const char * condition);
	virtual void onElse();
	virtual void onExitIf();

	virtual void onImport(const char * filename);

private:
	void genRead(int value, std::string & varName, int depth = 0);
	void genWrite(int value, std::string & varName);

	void genRead(TypeInfo * tinfo, std::string & varName);
	void genWrite(TypeInfo * tinfo, std::string & varName);

	const char * readFuncName(int value);
	const char * writeFuncName(int value);
	int typeSizeof(int value);
	const char * getInitStr(TypeInfo * tinfo);
	void setDepth(int dep = 0);

private:
	std::string __m_file;
	std::vector<std::string> __m_vecVar;
	std::string __m_structName;
	bool __m_isRead = false;
	int __m_depth = 0;
};

