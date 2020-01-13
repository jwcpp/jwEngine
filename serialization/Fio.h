/************************************************************************
* @file      Fio.h
* @brief     文件io相关
* @author    jiangwang
* @data      2019-12-25
* @version   0.1
************************************************************************/

#pragma once
#include <stdio.h>
#include <string>

class FBuffer
{
public:
	FBuffer();
	~FBuffer();

	bool load(const char * path);
	char * getBuffer(){ return __m_buffer; }
	const std::string & getFileName(){ return __m_filename; }
	char * currPointer(){ return __m_p; }
	char * lastPointer(){ return __m_lastp; }
	void   updateLast(){ __m_lastp = __m_p; };
	int getLine(){ return __m_nline; }
	char current(){ return __m_current; }
	void nextline();
	char next();
	bool currIsNewline() { return (__m_current == '\n' || __m_current == '\r'); }
private:
	char * __m_buffer = NULL;
	char * __m_p;
	char * __m_lastp;
	char   __m_current;
	int    __m_nline;
	std::string __m_filename;
};


std::string getFileNameByFilePath(const std::string filepath);
std::string getFileDir(const std::string filepath);

