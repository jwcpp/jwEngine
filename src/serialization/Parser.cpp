#include "Parser.h"
#include "Common.h"
#include "Token.h"
#include "Keyword.h"
#include "Typedef.h"
#include "Event.h"
#include "Fio.h"
#include <assert.h>

std::map<std::string, TypeInfo> gVars;

Parser::Parser(Token * token, Event * ev):
	__m_Token(token),
	__m_Event(ev)
{
}


Parser::~Parser()
{
}


void Parser::start()
{
	NEXT_TOKEN;
	for (;;)
	{
		switch (P_TOKEN->getToken())
		{
		case ';':
			NEXT_TOKEN;
			break;

		case eKw_EOF:
			return;

		case eKw_TYPEDEF:
			__typedef();
			break;

		case eKw_STRUCT:
			__struct();
			break;

		case eKw_IMPORT:
			__import();
			break;

		default:
			TOKEN_ERROR;
			break;
		}
	}

}

void Parser::__baseType(TypeInfo * tinfo)
{
	int token = P_TOKEN->getToken();
	tinfo->value = token;
	tinfo->name = P_TOKEN->getName();
	if (Keyword::isBaseType(token) || Keyword::isStrType(token))
	{
		// base type
	}
	else if (token == eKw_VAR)
	{
		TypeInfo * info = Typedef::getTypeInfo(P_TOKEN->getName().c_str());
		if (info)
		{
			if (info->type == 1)
			{
				//vector
				*tinfo = *info;
				tinfo->vec_name = P_TOKEN->getName();
			}
			else
			{
				tinfo->type = info->type;
				tinfo->value = info->value;
			}
		}
		else if (Typedef::isStruct(tinfo->name.c_str()))
		{
			// struct
			tinfo->value = eKw_STRUCT;
		}
		else
		{
			TOKEN_ERROR;
		}
	}
	else
	{
		TOKEN_ERROR;
	}
}

void Parser::__dataType(TypeInfo * tinfo)
{
	int token = P_TOKEN->getToken();
	if (token == eKw_VECTOR)
	{
		NEXT_TOKEN;
		MATCH_TOKEN('<');

		__baseType(tinfo);
		if (tinfo->type == 1)
		{
			// vector里面不允许直接包含vector
			TOKEN_ERROR;
		}
		tinfo->type = 1;

		NEXT_TOKEN;
		MATCH_TOKEN('>');

	}
	else
	{
		__baseType(tinfo);
		NEXT_TOKEN;
	}
}

void Parser::__typedef()
{
	NEXT_TOKEN;

	TypeInfo tinfo;
	__dataType(&tinfo);

	if (CHECK_TOKEN(eKw_VAR))
	{
		Typedef::addTypeInfo(P_TOKEN->getName().c_str(), tinfo);
		if (__m_Event)
			__m_Event->onTypedef(&tinfo, P_TOKEN->getName().c_str());
	}
	else
	{
		TOKEN_ERROR;
	}

	NEXT_TOKEN;
}

void Parser::__rwList()
{
	// [a,b,c]
	while (1)
	{
		if(CHECK_TOKEN(eKw_VAR))
		{
			std::string varname = P_TOKEN->getName();
			if (gVars.find(varname) == gVars.end())
			{
				TOKEN_ERROR;
			}
			NEXT_TOKEN;
			if (__m_Event)
				__m_Event->onReadWriteVar(&gVars[varname], varname.c_str());
		}
		else
		{
			TOKEN_ERROR;
		}

		if (CHECK_TOKEN(']'))
		{
			break;
		}

		MATCH_TOKEN(',');
	}
}

void Parser::__function()
{
	while (1)
	{
		switch (P_TOKEN->getToken())
		{
		case ';':
			NEXT_TOKEN;
			break;
		case '[':
			NEXT_TOKEN;
			__rwList();
			NEXT_TOKEN;
			break;
		case eKw_VAR:
		{
			//a = 10
			std::string varname = P_TOKEN->getName();
			if (gVars.find(varname) == gVars.end())
			{
				TOKEN_ERROR;
			}
			NEXT_TOKEN;
			MATCH_TOKEN('=');
			std::string exp;
			expre(exp);
			if (__m_Event)
				__m_Event->onValue(varname.c_str(), exp.c_str());
			break;
		}
		case eKw_IF:
		{
			if (__m_Event)
				__m_Event->onEnterIf();
		if_tiaojian:
			NEXT_TOKEN;
			MATCH_TOKEN('(');
			std::string exp;
			expre(exp);
			MATCH_TOKEN(')');
			if (__m_Event)
				__m_Event->onIfCond(exp.c_str());

		if_area:

			MATCH_TOKEN('{');
			// enter if module
			__function();
			MATCH_TOKEN('}');

			if (CHECK_TOKEN(eKw_ELSE))
			{
				NEXT_TOKEN;
				if (CHECK_TOKEN(eKw_IF))
				{
					if (__m_Event)
						__m_Event->onElseIf();
					goto if_tiaojian;
				}
				else 
				{
					if (__m_Event)
						__m_Event->onElse();
					goto if_area;
				}
			}

			if (__m_Event)
				__m_Event->onExitIf();
			break;
		}

		default:
			return;
		}
	}
}
void Parser::__struct()
{
	NEXT_TOKEN;
	if (CHECK_TOKEN(eKw_VAR))
	{
		Typedef::addStruct(P_TOKEN->getName().c_str());
		if (__m_Event)
			__m_Event->onEnterStruct(P_TOKEN->getName().c_str());
	}
	else
	{
		TOKEN_ERROR;
	}
	NEXT_TOKEN;
	MATCH_TOKEN('{');

	gVars.clear();

	for (;;)
	{

		switch (P_TOKEN->getToken())
		{
		case ';':
			NEXT_TOKEN;
			break;
		case '}':
			goto struct_end;
			break;
		case eKw_READ:
			NEXT_TOKEN;
			MATCH_TOKEN('{');
			if (__m_Event)
				__m_Event->onEnterRead();
			__function();
			if (__m_Event)
				__m_Event->onExitRead();
			MATCH_TOKEN('}');
			break;
		case eKw_WRITE:
			NEXT_TOKEN;
			MATCH_TOKEN('{');
			if (__m_Event)
				__m_Event->onEnterWrite();
			__function();
			if (__m_Event)
				__m_Event->onExitWrite();
			MATCH_TOKEN('}');
			break;
		default:
			TypeInfo tinfo;
			__dataType(&tinfo);

			if (CHECK_TOKEN(eKw_VAR))
			{
				std::string varname = P_TOKEN->getName();
				gVars[varname] = tinfo;
				NEXT_TOKEN;
				if (CHECK_TOKEN('='))
				{
					NEXT_TOKEN;
					std::string exp;
					expre(exp);
					if (__m_Event)
						__m_Event->onVariable(&tinfo, varname.c_str(), exp.c_str());
				}
				else
				{
					if (__m_Event)
						__m_Event->onVariable(&tinfo, varname.c_str(), "");
				}
			}
			else
			{
				TOKEN_ERROR;
			}
			break;
		}
	}

struct_end:

	if (__m_Event)
		__m_Event->onExitStruct();

	NEXT_TOKEN;
}

void Parser::__import()
{
	std::string filename;
	while (true)
	{
		NEXT_TOKEN;
		int token = P_TOKEN->getToken();
		if (CHECK_TOKEN(eKw_VAR))
		{
			filename.append(P_TOKEN->getName());
		}
		else if (token == '.')
		{
			filename.append(".");
		}
		else if (token == '/')
		{
			filename.append("/");
		}
		else
		{
			break;
		}
	}

	std::string dir = getFileDir(P_TOKEN->getBuffer()->getFileName());
	filename.insert(0, dir);

	// 解析文件，并将数据加入到Typedef中
	FBuffer fbuff;
	if (!fbuff.load(filename.c_str()))
	{
		printf("load file:%s error\n", filename.c_str());
		assert(0);
	}
	Token token(&fbuff);
	Parser parser(&token,NULL);
	parser.start();

	if (__m_Event){
		std::string name = getFileNameByFilePath(filename);
		__m_Event->onImport(name.c_str());
	}
}

Oper Parser::getOper(int op)
{
	switch (op)
	{
	case '+': return eOpr_Add;
	case '-': return eOpr_Sub;
	case '*': return eOpr_Mul;
	case '%': return eOpr_Mod;
	case '^': return eOpr_Pow;
	case '/': return eOpr_Div;
	case '&': return eOpr_Band;
	case '|': return eOpr_Bor;
	case '<': return eOpr_LT;
	case '>': return eOpr_GT;

	case eKw_EQ: return eOpr_EQ; // ==
	case eKw_NE: return eOpr_NE;	// !=
	case eKw_AND: return eOpr_AND;// &&
	case eKw_OR: return eOpr_OR; // ||
	case eKw_LE: return eOpr_LE; // <=
	case eKw_GE: return eOpr_GE; // >=
	case eKw_SHL: return eOpr_SHL; // <<
	case eKw_SHR: return eOpr_SHR; // >> 

	default: return eOpr_None;
	}
}

bool Parser::expre(std::string & str)
{
	// -a * b + c

	if (TEST_TOKEN('-'))
		str.append("-");

	while (1)
	{
		CHECK_TOKEN(eKw_VAR) || CHECK_TOKEN(eKw_NUM) || CHECK_TOKEN(eKw_DECIMALS) || TOKEN_ERROR;
		str.append(getExpreVar(P_TOKEN->getName()));
		NEXT_TOKEN;

		Oper oper = getOper(P_TOKEN->getToken());
		if (oper == eOpr_None)
			break;
		str.append(getOperStr(oper));
		P_TOKEN->nextToken();
	}

	return true;
}

std::string Parser::getExpreVar(const std::string & var)
{
	return var;
}

const char * Parser::getOperStr(Oper op)
{
	switch (op)
	{
	case eOpr_Add:
		return "+";
	case eOpr_Sub:
		return "-";
	case eOpr_Mul:
		return "*";
	case eOpr_Mod:
		return "%";
	case eOpr_Pow:
		return "^";
	case eOpr_Div:
		return "/";
	case eOpr_Band:
		return "&";
	case eOpr_Bor:
		return "|";
	case eOpr_LT:
		return "<";
	case eOpr_GT:
		return ">";

	case eOpr_EQ: // ==
		return "==";
	case eOpr_NE:	// !=
		return "!=";
	case eOpr_AND: // &&
		return "&&";
	case eOpr_OR: // ||
		return "||";
	case eOpr_LE: // <=
		return "<=";
	case eOpr_GE: // >=
		return ">=";
	case eOpr_SHL: // <<
		return "<<";
	case eOpr_SHR: // >> 
		return ">>";
	default:
		break;
	}

	return NULL;
}




ParserLua::ParserLua(Token * token, Event * ev):
	Parser(token, ev)
{

}
const char * ParserLua::getOperStr(Oper op)
{

	/*
		c++ ------> lua
		!=			~=
		&&			and
		||			or

	*/
	switch (op)
	{
	case eOpr_NE:	// !=
		return "~=";
	case eOpr_AND: // &&
		return " and ";
	case eOpr_OR: // ||
		return " or ";
	default:
		break;
	}

	return Parser::getOperStr(op);
}

std::string ParserLua::getExpreVar(const std::string & var)
{
	// member variable
	if (gVars.find(var) != gVars.end())
	{
		return std::string("self." + var);
	}

	return var;
}