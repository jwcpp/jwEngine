#include <stdio.h>
#include <stdlib.h>
#include "Token.h"
#include "Fio.h"
#include <assert.h>
#include "Parser.h"
#include "Event.h"
#include "Typedef.h"
#include "Generate.h"

#include <fstream>
#include <iostream>


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
	Generate gen;
	gen.init(name.c_str());

	Token token(&fbuff);

	Parser parser(&token, &gen);
	parser.start();

	{
		// write h file
		std::ofstream fout(dir + name + ".h");
		if (!fout)
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
		if (!fout)
		{
			std::cout << "文件不能打开" << std::endl;
		}
		else
		{
			fout << gen.getCppfile() << std::endl;
			fout.close();
		}
	}

	//system("pause");
	return 0;
}