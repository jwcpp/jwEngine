#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Token.h"
#include "Fio.h"
#include "Parser.h"
#include "Typedef.h"

#include <fstream>
#include <iostream>


#ifdef GEN_CPP

#include "Generate.h"

void start(std::string & dir, std::string & name, Token * token)
{
	Generate gen;
	gen.init(name.c_str());
	Parser parser(token, &gen);
	parser.start();

	{
		// write h file
		std::ofstream fout(dir + name + ".h");
		if (!fout.is_open())
		{
			std::cout << "文件不能打开" << std::endl;
		}
		else
		{
			fout << gen.getHfile() << std::endl;
			fout.close();
		}
	}
	{
		// write cpp file
		std::ofstream fout(dir + name + ".cpp");
		if (!fout.is_open())
		{
			std::cout << "文件不能打开" << std::endl;
		}
		else
		{
			fout << gen.getCppfile() << std::endl;
			fout.close();
		}
	}
}

#else

#include "GenerateLua.h"

void start(std::string & dir, std::string & name, Token * token)
{
	GenerateLua gen;
	gen.init();
	ParserLua parser(token, &gen);
	parser.start();

	{
		std::ofstream fout(dir + name + ".lua");
		if (!fout.is_open())
		{
			std::cout << "文件不能打开" << std::endl;
		}
		else
		{
			fout << gen.getfile() << std::endl;
			fout.close();
		}
	}
}

#endif


int main(int argc, char* argv[])
{
	const char * filename = argv[1];

	FBuffer fbuff;
	if (!fbuff.load(filename))
	{
		printf("load file:%s error\n", filename);
		assert(0);
	}

	std::string dir = getFileDir(filename);
	std::string name = getFileNameByFilePath(filename);


	Token token(&fbuff);
	
	start(dir, name, &token);
	

	//system("pause");
	return 0;
}