#include "Generate.h"
#include "Typedef.h"
#include "Keyword.h"


std::string gStructName;
bool gIsRead = false;
int gDepth = 0;


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

	__m_cpp.append("#include \"");
	__m_cpp.append(filename);
	__m_cpp.append(".h\" \n");
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
	gStructName = name;
	gDepth = 1;

	__m_h.append("struct ");
	__m_h.append(name);
	__m_h.append("\n{\n");
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
	gIsRead = true;
	__m_h.append("\t");
	__m_h.append("void read(ByteBuffer * buffer);\n");

	__m_cpp.append("void " + gStructName + "::read(ByteBuffer * buffer)\n{\n");
}
void Generate::onExitRead()
{
	__m_cpp.append("}\n");
}

void Generate::onEnterWrite()
{
	gIsRead = false;
	__m_h.append("\t");
	__m_h.append("void write(ByteBuffer * buffer);\n");

	__m_cpp.append("void " + gStructName + "::write(ByteBuffer * buffer)\n{\n");
}

void Generate::onExitWrite()
{
	__m_cpp.append("}\n");
}
void Generate::onValue(const char * varName, const char * exp)
{
	setDepth();
	__m_cpp.append(varName);
	__m_cpp.append(" = ");
	__m_cpp.append(exp);
	__m_cpp.append("; \n");
}

void Generate::genBaseType(TypeInfo * tinfo, const char * varName)
{
	if (gIsRead)
	{
		if (Keyword::isBaseType(tinfo->value))
		{
			__m_cpp.append("*buffer >> ");
			__m_cpp.append(varName);
		}
		else
		{
			__m_cpp.append(varName);
			__m_cpp.append(".read(buffer)");
		}
	}
	else
	{
		if (Keyword::isBaseType(tinfo->value))
		{
			__m_cpp.append("*buffer << ");
			__m_cpp.append(varName);
		}
		else
		{
			__m_cpp.append(varName);
			__m_cpp.append(".write(buffer)");
		}
	}

	__m_cpp.append(";\n");
}
void Generate::onReadWriteVar(TypeInfo * tinfo, const char * varName)
{
	if (tinfo->type == 1)
	{
		std::string len_var = "len_";
		len_var.append(varName);

		if (gIsRead)
		{
			setDepth();
			__m_cpp.append("int " + len_var + " = 0;\n");
			setDepth();
			__m_cpp.append("*buffer >> " + len_var + ";\n");
			setDepth();
			__m_cpp.append("for( int i = 0; i< " + len_var + "; ++i) { \n");

			std::string r_var = "temp_";
			r_var += varName;
			setDepth(+1);
			__m_cpp.append(tinfo->name + " " + r_var + ";\n");
			setDepth(+1);
			genBaseType(tinfo, r_var.c_str());
			setDepth(+1);
			__m_cpp.append(varName);
			__m_cpp.append(".push_back(" + r_var + ");\n");

			setDepth();
			__m_cpp.append("}\n");
		}
		else
		{
			setDepth();
			__m_cpp.append("int " + len_var + " = " + varName + ".size();\n");
			setDepth();
			__m_cpp.append("*buffer << " + len_var+ "; \n");
			setDepth();
			__m_cpp.append("for( int i = 0; i< " + len_var + "; ++i) { \n");

			std::string w_var = varName;
			w_var += "[i]";
			setDepth(+1);
			genBaseType(tinfo, w_var.c_str());

			setDepth();
			__m_cpp.append("}\n");
		}
	}
	else
	{
		setDepth();
		genBaseType(tinfo, varName);
	}
}

void Generate::onEnterIf()
{
	setDepth();
	__m_cpp.append("if");
}

void Generate::onElseIf()
{
	gDepth--;
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
	gDepth++;
}
void Generate::onElse()
{
	gDepth--;
	__m_cpp.append("\n");
	setDepth();
	__m_cpp.append("}else{\n");
	gDepth++;
}

void Generate::onExitIf()
{
	gDepth--;
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
	for (int i = 0; i < gDepth + dep; ++i)
	{
		__m_cpp.append("\t");
	}
}