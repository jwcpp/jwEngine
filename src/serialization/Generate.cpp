#include "Generate.h"
#include "Typedef.h"
#include "Keyword.h"
#include <cstring>


std::string getStdName(std::string name)
{
	if (name == "string")
		return "std::string";

	return name;
}

Generate::Generate()
{
}


Generate::~Generate()
{
}


void Generate::init(const char * filename)
{
	__m_h.append("#pragma once \n");
	__m_h.append("\n");
	__m_h.append("#include \"BaseType.h\" \n");
	__m_h.append("#include <vector> \n");
	__m_h.append("class ByteBuffer;\n\n");

	__m_cpp.append("#include \"" + std::string(filename) + ".h\" \n");
	__m_cpp.append("#include \"ByteBuffer.h\" \n");
}

void Generate::onTypedef(TypeInfo * tinfo, const char * name)
{
	if (tinfo->type)
	{
		if (tinfo->vec_name.size() > 0)
		{
			__m_h.append("typedef " + tinfo->vec_name + " " + name + ";\n");
		}
		else
		{
			__m_h.append("typedef std::vector<" + getStdName(tinfo->name) + "> " + name + ";\n");
		}
	}
	else
	{
		__m_h.append("typedef " + getStdName(tinfo->name) + " " + name + ";\n");
	}
}

void Generate::onEnterStruct(const char * name)
{
	__m_structName = name;

	__m_h.append("struct " + std::string(name) + "\n{\n");
	__m_h.append("public:\n");
}
void Generate::onExitStruct()
{
	__m_h.append("}; \n");
}

void Generate::onVariable(TypeInfo * tinfo, const char * varName, const char * exp)
{
	__m_h.append("\t");

	if (tinfo->type == 1)
	{
		if (tinfo->vec_name.size() > 0)
		{
			__m_h.append(tinfo->vec_name + " " + varName);
		}
		else
		{
			__m_h.append("std::vector<" + getStdName(tinfo->name) + "> " + varName);
		}
	}
	else
	{
		
		__m_h.append(getStdName(tinfo->name) + " " + varName);

		if (exp && strlen(exp) > 0)
		{
			__m_h.append(" = ");
			__m_h.append(exp);
		}
	}

	__m_h.append("; \n");
}

void Generate::onEnterRead()
{
	__m_isRead = true;
	__m_h.append("\t");
	__m_h.append("bool read(ByteBuffer * buffer);\n");

	__m_cpp.append("bool " + __m_structName + "::read(ByteBuffer * buffer)\n{\n");
	__m_depth++;
}
void Generate::onExitRead()
{
	setDepth();
	__m_cpp.append("return true;\n");
	__m_cpp.append("}\n");
	__m_depth--;
}

void Generate::onEnterWrite()
{
	__m_isRead = false;
	__m_h.append("\t");
	__m_h.append("void write(ByteBuffer * buffer);\n");

	__m_cpp.append("void " + __m_structName + "::write(ByteBuffer * buffer)\n{\n");
	__m_depth++;
}

void Generate::onExitWrite()
{
	__m_cpp.append("}\n");
	__m_depth--;
}
void Generate::onValue(const char * varName, const char * exp)
{
	setDepth();
	__m_cpp.append(varName);
	__m_cpp.append(" = ");
	__m_cpp.append(exp);
	__m_cpp.append("; \n");
}


void Generate::genRead(int value, std::string & varName, int depth)
{
	if (Keyword::isBaseType(value))
	{
		__m_cpp.append("if(sizeof(" + varName + ") + buffer->rpos() > buffer->wpos()) return false;\n");
		setDepth(depth);
		__m_cpp.append("*buffer >> " + varName);
	}
	else if (Keyword::isStrType(value))
	{
		//The string ends with '\0'
		__m_cpp.append("*buffer >> " + varName);
	}
	else
	{
		__m_cpp.append("if(" + varName + ".read(buffer) == false) return false");
	}

	__m_cpp.append(";\n");
}

void Generate::genRead(TypeInfo * tinfo, std::string & varName)
{
	// vector
	if (tinfo->type == 1)
	{
		std::string len_var = "len_" + varName;
		std::string r_var = "temp_" + varName;

		setDepth();
		__m_cpp.append("int " + len_var + ";\n");
		setDepth();
		genRead(eKw_INT32, len_var);

		setDepth();
		__m_cpp.append("for( int i = 0; i< " + len_var + "; ++i) { \n");

		setDepth(+1);
		__m_cpp.append(getStdName(tinfo->name) + " " + r_var + ";\n");
		setDepth(+1);
		genRead(tinfo->value, r_var, 1);
		setDepth(+1);
		__m_cpp.append(varName + ".push_back(" + r_var + ");\n");

		setDepth();
		__m_cpp.append("}\n");
	}
	else
	{
		setDepth();
		genRead(tinfo->value, varName);
	}
}

void Generate::genWrite(int value, std::string & varName)
{
	if (Keyword::isBaseType(value) || Keyword::isStrType(value))
	{
		__m_cpp.append("*buffer << " + varName);
	}
	else
	{
		__m_cpp.append(varName + ".write(buffer)");
	}

	__m_cpp.append(";\n");
}

void Generate::genWrite(TypeInfo * tinfo, std::string & varName)
{
	// vector
	if (tinfo->type == 1)
	{
		std::string len_var = "len_" + varName;

		setDepth();
		__m_cpp.append("int " + len_var + " = " + varName + ".size();\n");
		setDepth();
		genWrite(eKw_INT32, len_var);
		setDepth();
		__m_cpp.append("for( int i = 0; i< " + len_var + "; ++i) { \n");

		std::string w_var = varName + "[i]";
		setDepth(+1);
		genWrite(tinfo->value, w_var);

		setDepth();
		__m_cpp.append("}\n");
	}
	else
	{
		setDepth();
		genWrite(tinfo->value, varName);
	}
}

void Generate::onReadWriteVar(TypeInfo * tinfo, const char * varName)
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

void Generate::onEnterIf()
{
	setDepth();
	__m_cpp.append("if");
}

void Generate::onElseIf()
{
	__m_depth--;
	setDepth();
	__m_cpp.append("} else if");
}

void Generate::onIfCond(const char * condition)
{
	__m_cpp.append("(");
	__m_cpp.append(condition);
	__m_cpp.append(")\n");

	setDepth();
	__m_cpp.append("{\n");
	__m_depth++;
}
void Generate::onElse()
{
	__m_depth--;
	__m_cpp.append("\n");
	setDepth();
	__m_cpp.append("}else{\n");
	__m_depth++;
}

void Generate::onExitIf()
{
	__m_depth--;
	__m_cpp.append("\n");
	setDepth();
	__m_cpp.append("}\n");
}

void Generate::onImport(const char * filename)
{
	__m_h.append("#include \"");
	__m_h.append(filename);
	__m_h.append(".h\"\n");
}

void Generate::setDepth(int dep)
{
	for (int i = 0; i < __m_depth + dep; ++i)
	{
		__m_cpp.append("\t");
	}
}