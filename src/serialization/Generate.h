/************************************************************************
* @file      Generate.h
* @brief     Éú³Éc++´úÂë
* @author    jiangwang
* @data      2019-1-10
* @version   0.1
************************************************************************/

#pragma once

#include "Event.h"
#include <string>
class Generate : public Event
{
public:
	Generate();
	~Generate();

	void init(const char * filename);
	std::string getHfile(){ return __m_h; }
	std::string getCppfile(){ return __m_cpp; }

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

	void setDepth(int dep = 0);
private:
	std::string __m_h;
	std::string __m_cpp;

	std::string __m_structName;
	bool __m_isRead = false;
	int __m_depth = 0;
};

