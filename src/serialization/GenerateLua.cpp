#include "GenerateLua.h"
#include "Typedef.h"
#include "Keyword.h"
#include <cstring>

void GenerateLua::init()
{

}

void GenerateLua::onTypedef(TypeInfo * tinfo, const char * name)
{
	if (tinfo->value == eKw_STRUCT)
	{
		__m_file.append(name);
		__m_file.append(" = ");
		__m_file.append(tinfo->name);
		__m_file.append("\n");
	}
}


void GenerateLua::onEnterStruct(const char * name)
{
	__m_structName = name;
	__m_depth = 0;

	__m_file.append(name);
	__m_file.append(" = {}\n");
	__m_file.append(__m_structName + ".__index = " + __m_structName + "\n");
}
void GenerateLua::onExitStruct()
{

	__m_file.append("function " + __m_structName + ":new()\n");

	setDepth(1);
	__m_file.append("local o = {}\n");
	setDepth(1);
	__m_file.append("setmetatable(o, " + __m_structName + ")\n");
	
	for (int i = 0; i < __m_vecVar.size(); ++i)
	{
		setDepth(1);
		__m_file.append("o." + __m_vecVar[i] + "\n");
	}
	__m_vecVar.clear();

	setDepth(1);
	__m_file.append("return o\n");

	__m_file.append("end");

	__m_file.append("\n\n");
}

void GenerateLua::onVariable(TypeInfo * tinfo, const char * varName, const char * exp)
{
	//setDepth();
	std::string tempstr;
	if (tinfo->type == 1)
	{
		tempstr.append(std::string(varName) + " = {}");
	}
	else
	{

		tempstr.append(varName);

		if (exp && strlen(exp) > 0)
		{
			tempstr.append(" = ");
			tempstr.append(exp);
		}
		else
		{
			tempstr.append(" = ");
			tempstr.append(getInitStr(tinfo));
		}
	}

	__m_vecVar.push_back(tempstr);
}

void GenerateLua::onEnterRead()
{
	setDepth();
	__m_isRead = true;
	__m_file.append("function " + __m_structName + ":read(buffer)\n");

	__m_depth += 1;
}
void GenerateLua::onExitRead()
{
	setDepth();
	__m_file.append("return true;\n");
	__m_file.append("end\n");

	__m_depth -= 1;
}

void GenerateLua::onEnterWrite()
{
	setDepth();
	__m_isRead = false;
	__m_file.append("function " + __m_structName + ":write(buffer)\n");

	__m_depth += 1;
}

void GenerateLua::onExitWrite()
{
	__m_file.append("end\n");

	__m_depth -= 1;
}

void GenerateLua::onValue(const char * varName, const char * exp)
{
	setDepth();
	__m_file.append(varName);
	__m_file.append(" = ");
	__m_file.append(exp);
	__m_file.append("\n");
}


void GenerateLua::genRead(int value, std::string & varName, int depth)
{
	if (Keyword::isBaseType(value))
	{
		__m_file.append("if " + std::to_string(typeSizeof(value)) + " + buffer:rpos() > buffer:wpos() then return false end\n");
		setDepth(depth);
		__m_file.append(varName + " = buffer:" + readFuncName(value) + "()");
	}
	else if (Keyword::isStrType(value))
	{
		//The string ends with '\0'
		__m_file.append(varName + " = buffer:" + readFuncName(value) + "()");
	}
	else
	{
		__m_file.append("if " + varName + ":read(buffer) == false then return false end");
	}

	__m_file.append(";\n");
}

void GenerateLua::genWrite(int value, std::string & varName)
{
	if (Keyword::isBaseType(value) || Keyword::isStrType(value))
	{
		__m_file.append("buffer:" + std::string(writeFuncName(value)) + "(" + varName + ")");
	}
	else
	{
		__m_file.append(varName + ":write(buffer)");
	}

	__m_file.append(";\n");
}

void GenerateLua::genRead(TypeInfo * tinfo, std::string & varName)
{
	std::string self_var = "self." + varName;

	if (tinfo->type == 1)
	{
		std::string len_var = "len_" + varName;
		std::string r_var = "temp_" + varName;

		setDepth();
		__m_file.append("local " + len_var + " = 0\n");
		setDepth();
		__m_file.append("local " + r_var + " = nil\n");

		setDepth();
		genRead(eKw_INT32, len_var);

		setDepth();
		__m_file.append("for i = 1, " + len_var + ", 1 do\n");

		if (tinfo->value == eKw_STRUCT)
		{
			setDepth(+1);
			__m_file.append(r_var + " = " + tinfo->name + ":new()\n");
		}

		setDepth(+1);
		genRead(tinfo->value, r_var, 1);

		setDepth(+1);
		__m_file.append(self_var + "[i] = " + r_var + "\n");

		setDepth();
		__m_file.append("end\n");
	}
	else
	{
		if (tinfo->value == eKw_STRUCT)
		{
			setDepth();
			__m_file.append(self_var + " = " + tinfo->name + ":new()\n");
		}

		setDepth();
		genRead(tinfo->value, self_var);
	}
}
void GenerateLua::genWrite(TypeInfo * tinfo, std::string & varName)
{
	std::string self_var = "self." + varName;

	if (tinfo->type == 1)
	{
		std::string len_var = "len_" + varName;
		std::string w_var = self_var + "[i]";

		setDepth();
		__m_file.append("local " + len_var + " = #" + self_var + "\n");
		setDepth();
		genWrite(eKw_INT32, len_var);
		setDepth();
		__m_file.append("for i = 1, " + len_var + ", 1 do \n");

		setDepth(+1);
		genWrite(tinfo->value, w_var);

		setDepth();
		__m_file.append("end\n");
	}
	else
	{
		setDepth();
		genWrite(tinfo->value, self_var);
	}
}

void GenerateLua::onReadWriteVar(TypeInfo * tinfo, const char * varName)
{
	std::string tempstr(varName);
	if (__m_isRead)
	{
		genRead(tinfo, tempstr);
	}
	else
	{
		genWrite(tinfo, tempstr);
	}
}

void GenerateLua::onEnterIf()
{
	setDepth();
	__m_file.append("if");
}

void GenerateLua::onElseIf()
{
	__m_depth--;
	setDepth();
	__m_file.append("elseif");
}

void GenerateLua::onIfCond(const char * condition)
{
	__m_file.append(" ");
	__m_file.append(condition);
	__m_file.append(" then\n");

	__m_depth++;
}
void GenerateLua::onElse()
{
	__m_depth--;
	__m_file.append("\n");
	setDepth();
	__m_file.append("else\n");
	__m_depth++;
}

void GenerateLua::onExitIf()
{
	__m_depth--;
	__m_file.append("\n");
	setDepth();
	__m_file.append("end\n");
}

void GenerateLua::onImport(const char * filename)
{
	__m_file.append("require");
	__m_file.append("(\"");
	__m_file.append(filename);
	__m_file.append("\")\n");
}

const char * GenerateLua::readFuncName(int value)
{
	switch (value)
	{
	case eKw_INT8: return "getInt8";
	case eKw_UINT8: return "getUint8";
	case eKw_INT16: return "getInt16";
	case eKw_UINT16: return "getUint16";
	case eKw_INT32: return "getInt32";
	case eKw_UINT32: return "getUint32";
	case eKw_INT64: return "getInt64";
	case eKw_UINT64: return "getUint64";
	case eKw_FLOAT: return "getFloat";
	case eKw_DOUBLE: return "getDouble";
	case eKw_STRING: return "getString";
	default: return "";
	}
}
const char * GenerateLua::writeFuncName(int value)
{
	switch (value)
	{
	case eKw_INT8: return "pushInt8";
	case eKw_UINT8: return "pushUint8";
	case eKw_INT16: return "pushInt16";
	case eKw_UINT16: return "pushUint16";
	case eKw_INT32: return "pushInt32";
	case eKw_UINT32: return "pushUint32";
	case eKw_INT64: return "pushInt64";
	case eKw_UINT64: return "pushUint64";
	case eKw_FLOAT: return "pushFloat";
	case eKw_DOUBLE: return "pushDouble";
	case eKw_STRING: return "pushString";
	default: return "";
	}
}

int GenerateLua::typeSizeof(int value)
{
	switch (value)
	{
	case eKw_INT8: return 1;
	case eKw_UINT8: return 1;
	case eKw_INT16: return 2;
	case eKw_UINT16: return 2;
	case eKw_INT32: return 4;
	case eKw_UINT32: return 4;
	case eKw_INT64: return 8;
	case eKw_UINT64: return 8;
	case eKw_FLOAT: return sizeof(float);
	case eKw_DOUBLE: return sizeof(double);
	default: return 0;
	}
}

const char * GenerateLua::getInitStr(TypeInfo * tinfo)
{
	switch (tinfo->value)
	{
	case eKw_INT8:
	case eKw_UINT8:
	case eKw_INT16:
	case eKw_UINT16:
	case eKw_INT32:
	case eKw_UINT32:
	case eKw_INT64:
	case eKw_UINT64:
	case eKw_FLOAT:
	case eKw_DOUBLE:
		return "0";
		break;
	case eKw_STRING:
		return "\"\"";
	case eKw_STRUCT:
	default:
		return "{}";
	}
}

void GenerateLua::setDepth(int dep)
{
	for (int i = 0; i < __m_depth + dep; ++i)
	{
		__m_file.append("\t");
	}
}