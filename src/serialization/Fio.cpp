#include "Fio.h"
#include <stdlib.h>


FBuffer::FBuffer()
{
	__m_nline = 1;
	__m_buffer = NULL;
}


FBuffer::~FBuffer()
{
	if (__m_buffer)
		free(__m_buffer);
}


bool FBuffer::load(const char * path)
{
	FILE *pf = fopen(path, "rb");
	if (!pf)
		return false;
	fseek(pf, 0, SEEK_END);
	long lSize = ftell(pf);
	__m_buffer = (char*)malloc(lSize + 1);
	if (!__m_buffer)
		return false;
	rewind(pf); //指针重新指向文件开始
	fread(__m_buffer, sizeof(char), lSize, pf);
	__m_buffer[lSize] = '\0';
	fclose(pf);

	// set
	__m_p = __m_buffer;
	__m_lastp = __m_p;
	__m_filename = path;
	__m_current = *__m_p;

	return true;
}


void FBuffer::nextline()
{ 
	int old = __m_current;
	next();
	if (currIsNewline() && __m_current != old)
		next();
	__m_nline++;
}
char FBuffer::next()
{
	__m_p++;
	__m_current = *__m_p;
	return __m_current;
}



std::string getFileNameByFilePath(const std::string filepath)
{
	int l_idx = filepath.rfind('\\');
	if (l_idx == std::string::npos)
	{
		l_idx = filepath.rfind('/');
	}

	int r_idx = filepath.rfind('.');
	return filepath.substr(l_idx + 1, r_idx - l_idx - 1);
}

std::string getFileDir(const std::string filepath)
{
	int l_idx = filepath.rfind('\\');
	if (l_idx == std::string::npos)
	{
		l_idx = filepath.rfind('/');
	}
	return filepath.substr(0, l_idx + 1);
}