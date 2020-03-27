#include "GenerateLua.h"
#include "Typedef.h"
#include "Keyword.h"

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
	setDepth(1);
	__m_file.append("return o\n");

	__m_file.append("end");

	__m_file.append("\n\n");
}

void GenerateLua::onVariable(TypeInfo * tinfo, const char * varName, const char * exp)
{
	setDepth();

	if (tinfo->type == 1)
	{
		__m_file.append(__m_structName + ".");
		__m_file.append(varName);
		__m_file.append(" = {}");
	}
	else
	{

		__m_file.append(__m_structName + ".");
		__m_file.append(varName);

		if (exp && strlen(exp) > 0)
		{
			__m_file.append(" = ");
			__m_file.append(exp);
		}
		else
		{
			__m_file.append(" = ");
			__m_file.append(getInitStr(tinfo));
		}
	}

	__m_file.append("\n");
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

void GenerateLua::onReadWriteVar(TypeInfo * tinfo, const char * varName)
{
	std::string self_var = "self.";
	self_var += varName;

	if (tinfo->type == 1)
	{
		std::string len_var = "len_";
		len_var.append(varName);

		if (__m_isRead)
		{
			setDepth();
			__m_file.append(len_var + " = buffer:getInt32();\n");
			setDepth();
			__m_file.append("for i = 0, " + len_var + " - 1, 1 do\n");
			std::string r_var = "temp_";
			r_var += varName;

			if (tinfo->value == eKw_STRUCT)
			{
				setDepth(+1);
				__m_file.append(r_var + " = " + tinfo->name + ":new()\n");
			}

			setDepth(+1);
			genBaseType(tinfo, r_var.c_str());

			setDepth(+1);
			__m_file.append(self_var);
			__m_file.append("[#");
			__m_file.append(self_var);
			__m_file.append(" + 1] = " + r_var + "\n");

			setDepth();
			__m_file.append("end\n");
		}
		else
		{
			setDepth();
			__m_file.append(len_var + " = #" + self_var + "\n");
			setDepth();
			__m_file.append("buffer:pushInt32(" + len_var + ")\n");
			setDepth();
			__m_file.append("for i = 1, " + len_var + ", 1 do \n");

			std::string w_var = self_var;
			w_var += "[i]";
			setDepth(+1);
			genBaseType(tinfo, w_var.c_str());

			setDepth();
			__m_file.append("end\n");
		}
	}
	else
	{
		if (!__m_isRead)//write
		{
			setDepth();
			__m_file.append(varName);
			__m_file.append(" = " + self_var + "\n");
		}
		else if (tinfo->value == eKw_STRUCT)
		{
			setDepth();
			__m_file.append(varName);
			__m_file.append(" = " + tinfo->name + ":new()\n");
		}

		setDepth();
		genBaseType(tinfo, varName);

		if (__m_isRead)//read
		{
			setDepth();
			__m_file.append(self_var + " = " + varName + "\n");
		}
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

void GenerateLua::genBaseType(TypeInfo * tinfo, const char * varName)
{
	if (__m_isRead)
	{
		if (Keyword::isBaseType(tinfo->value))
		{
			__m_file.append(varName);
			__m_file.append(" = ");
			__m_file.append("buffer:");
			__m_file.append(readFuncName(tinfo->value));
			__m_file.append("()");
		}
		else
		{
			__m_file.append(varName);
			__m_file.append(":read(buffer)");
		}
	}
	else
	{
		if (Keyword::isBaseType(tinfo->value))
		{
			__m_file.append("buffer:");
			__m_file.append(writeFuncName(tinfo->value));
			__m_file.append("(");
			__m_file.append(varName);
			__m_file.append(")");
		}
		else
		{
			__m_file.append(varName);
			__m_file.append(":write(buffer)");
		}
	}

	__m_file.append(";\n");
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