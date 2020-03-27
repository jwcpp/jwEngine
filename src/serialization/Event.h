/************************************************************************
* @file      Event.h
* @brief     ½âÎöÊÂ¼þ
* @author    jiangwang
* @data      2019-1-10
* @version   0.1
************************************************************************/
#pragma once

class TypeInfo;
class Event
{
public:
	// typedef
	virtual void onTypedef(TypeInfo * tinfo, const char * name) = 0;

	virtual void onEnterStruct(const char * name) = 0;
	virtual void onExitStruct() = 0;
	virtual void onVariable(TypeInfo * tinfo, const char * varName, const char * exp) = 0;

	virtual void onEnterRead() = 0;
	virtual void onExitRead() = 0;
	virtual void onEnterWrite() = 0;
	virtual void onExitWrite() = 0;
	virtual void onValue(const char * varName, const char * exp) = 0;
	virtual void onReadWriteVar(TypeInfo * tinfo, const char * varName) = 0;

	virtual void onEnterIf() = 0;
	virtual void onElseIf() = 0;
	virtual void onIfCond(const char * condition) = 0;
	virtual void onElse() = 0;
	virtual void onExitIf() = 0;

	virtual void onImport(const char * filename) = 0;
};