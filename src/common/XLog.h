#pragma once

#include <string>
#include <sstream>
#include "SpinLock.h"

#define LOG_MAX_SIZE 1073741824

enum LogLevel
{
	LL_INFO,
	LL_WARNING,
	LL_ERROR,
};

class XLog
{
	class AutoLock;
public:
	XLog();
	~XLog();

	void init(const char * rootdir, const char * childdir);
	void setMaxsize(unsigned int size){ maxsize = size; }
	void writeLog(int level, const char * filename, const char * func, int line, const char * format, ...);
private:
	void writeFile(int level, const char * filename, const char * func, int line, const char * logstr);
	void closefile();
	void print(int level, std::string& str);
private:
	FILE * file;
	std::string logdir;
	std::string childdir;
	std::string currdir;
	int writeday;
	unsigned int maxsize;
	unsigned int currsize;
	SpinLock _lock;
};

class LogStream
{
public:
	LogStream(XLog * log, int level, const char * filename, const char * func, int line);
	~LogStream();

	template<typename T>
	LogStream & operator << (T value)
	{
		stream << value;
		return *this;
	}
private:
	std::stringstream stream;
	XLog * _log;
	int _level;
	std::string _filename;
	std::string _func;
	int _line;
};

extern XLog commonlog;

#define INIT_LOG(rootdir, childdir) commonlog.init(rootdir, childdir)

#define INFO_LOG(...) commonlog.writeLog(LL_INFO , __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define WARNING_LOG(...) commonlog.writeLog(LL_WARNING, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define ERROR_LOG(...) commonlog.writeLog(LL_ERROR, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define XLOG(level) LogStream(&commonlog, level, __FILE__, __FUNCTION__, __LINE__)
#define XLOG_IF(condition, level) if(condition)\
	LogStream(&commonlog, level, __FILE__, __FUNCTION__, __LINE__)

