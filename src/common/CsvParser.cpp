#include "CsvParser.h"
#include <fstream>
#include "XLog.h"


CsvParser::CsvParser(const char * file, int startline, char sep) :
	_sep(sep)
{
	std::string line;

	std::ifstream ifile(file);
	if (ifile.is_open())
	{
		int row = 0;
		while (ifile.good())
		{
			getline(ifile, line);
			if (line == "")
				continue;

			if (startline && --startline)
				continue;

			row++;
			if (row == 1)
			{
				parseHeader(line);
			}
			else
			{
				parseContent(line);
			}
		}
		ifile.close();
	}
	else
	{
		ERROR_LOG("Failed to open %s", file);
	}
}


CsvParser::~CsvParser()
{
}

void CsvParser::parseHeader(std::string & line)
{
	std::stringstream ss(line);
	std::string item;

	while (std::getline(ss, item, _sep))
		_header.push_back(item);
}

void CsvParser::parseContent(std::string & line)
{
	std::stringstream ss(line);
	std::string item;

	CsvRow csvrow;
	while (std::getline(ss, item, _sep))
		csvrow.push(item);

	_content.push_back(csvrow);
}

int CsvParser::row()
{
	return _content.size();
}