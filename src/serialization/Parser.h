#pragma once

/************************************************************************
* @file      Parser.h
* @brief     语法分析
* @author    jiangwang
* @data      2020-1-2
* @version   0.1
************************************************************************/
#include <string>

enum Oper {
	eOpr_Add, // +
	eOpr_Sub, // -
	eOpr_Mul, // *
	eOpr_Mod, // %
	eOpr_Pow, // ^
	eOpr_Div, // /
	eOpr_Band, // &
	eOpr_Bor, // |
	eOpr_LT,  // <
	eOpr_GT, // >

	eOpr_EQ, // ==
	eOpr_NE,	// !=
	eOpr_AND,// &&
	eOpr_OR, // ||
	eOpr_LE, // <=
	eOpr_GE, // >=
	eOpr_SHL, // <<
	eOpr_SHR, // >>

	eOpr_None,
};

class Token;
class TypeInfo;
class Event;
class Parser
{
public:
	Parser(Token * token, Event * ev);
	virtual ~Parser();

	void start();


protected:
	virtual const char * getOperStr(Oper op);
	virtual std::string getExpreVar(const std::string & var);
private:

	// Parse data type
	void __baseType(TypeInfo * info);
	void __dataType(TypeInfo * info);

	// typedef
	void __typedef();
	void __struct();
	void __rwList();
	void __function();

	// 表达式
	Oper getOper(int op);
	bool expre(std::string & str);

	// import
	void __import();

	Token * pToken(){ return __m_Token; }
private:
	Token * __m_Token;
	Event * __m_Event;
};


class ParserLua : public Parser
{
public:
	ParserLua(Token * token, Event * ev);
	virtual const char * getOperStr(Oper op);
	virtual std::string getExpreVar(const std::string & var);
};
