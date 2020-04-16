#pragma once
#include <string>
# include <sstream>
#include <vector>

class CsvRow
{
public:
	unsigned int size(void) const{ return _values.size(); }
	void push(const std::string & title){ _values.push_back(title); }

	template<typename T>
	const T getValue(int idx, T defvalue = T()) const
	{
		T res = defvalue;
		std::stringstream ss;
		ss << _values[idx];
		ss >> res;
		return res;
	}
private:
	std::vector<std::string> _values;
};

class CsvParser
{
public:
	CsvParser(const char * file, int startline = 1, char sep = '\t');
	~CsvParser();

	template<typename T>
	const T getValue(int row, std::string name, T defvalue = T()) const
	{
		for (unsigned int i = 0; i < _header.size();++i)
		{
			if (name == _header[i])
			{
				const CsvRow & csvrow = _content[row - 1];
				T ret = csvrow.getValue<T>(i, defvalue);
				return ret;
			}
		}
		
		return T();
	}

	int row();

protected:
	void parseHeader(std::string & line);
	void parseContent(std::string & line);
private:
	const char _sep;
	std::vector<std::string> _header;
	std::vector<CsvRow> _content;
};

