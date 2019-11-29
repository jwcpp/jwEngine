#include "XLog.h"
#include "XFile.h"
#include <time.h>
#include <iomanip>
#include <stdarg.h>
#include "Platform.h"

#ifdef SYSTEM_WIN
#include <windows.h>
#endif


XLog commonlog;

class XLog::AutoLock
{
public:
	AutoLock(XLog * log) :_log(log)
	{
		log->_lock.lock(); 
	}
	~AutoLock()
	{ 
		_log->_lock.unlock();
	}

	XLog * _log;
};

XLog::XLog():
	file(NULL),
	writeday(0),
	maxsize(LOG_MAX_SIZE),
	currsize(0)
{
}


XLog::~XLog()
{
	closefile();
}

void XLog::init(const char * dir)
{
	logdir = dir;
	XFile::createDirectory(dir);
}


void XLog::closefile()
{
	if (file)
	{
		fclose(file);
		file = NULL;
		currsize = 0;
	}
}

void XLog::writeLog(int level, const char * filename, const char * func, int row, const char * format, ...)
{
	va_list vl;
	va_start(vl, format);

	int len = vsnprintf(NULL, 0, format, vl);
	std::string s("", len + 1);
	char * buff = (char *)s.data();
	vsprintf(buff, format, vl);

	va_end(vl);

	writeFile(level, filename, func, row, s.c_str());
}

void XLog::writeFile(int level, const char * filename, const char * func, int row, const char * logstr)
{
	time_t t = time(NULL);
	struct tm * tm_ = std::localtime(&t);

	AutoLock(this);

	bool newfile = (file == NULL);
	if (tm_->tm_mday != writeday)
	{
		writeday = tm_->tm_mday;
		char dirname[12];
		sprintf(dirname, "%d-%.2d-%.2d", tm_->tm_year + 1900, tm_->tm_mon + 1, tm_->tm_mday);

		currdir = logdir;
		currdir.append("/");
		currdir.append(dirname);
		if (!XFile::existDir(currdir.c_str()))
		{
			XFile::mkdir(currdir.c_str());
		}

		newfile = true;
	}

	if (newfile || currsize >= maxsize)
	{
		closefile();
		char dirname[12];
		sprintf(dirname, "%.2d.%.2d.%.2d", tm_->tm_hour, tm_->tm_min, tm_->tm_sec);
		std::string filepath = currdir;
		filepath.append("/");
		filepath.append(dirname);
		filepath.append(".log");
		file = fopen(filepath.c_str(), "a+");
	}

	if (!file)
	{
		return;
	}

	std::string wstr;
	char buff[12];
	sprintf(buff, "%.2d:%.2d:%.2d", tm_->tm_hour, tm_->tm_min, tm_->tm_sec); //%.2d:%.2d:%.2d == 13:01:05
	//time
	wstr.append(buff);
	wstr.append("\t");

	//level
	switch (level)
	{
	case LL_INFO:
		wstr.append("info");
		wstr.append("\t");
		break;

	case LL_WARNING:
		wstr.append("warning");
		wstr.append("\t");
		break;
	case LL_ERROR:
		wstr.append("error");
		wstr.append("\t");
		break;

	default:
		break;
	}

	//function info
	wstr.append(filename);
	wstr.append("|");
	wstr.append(func);
	wstr.append("|");
	wstr.append(std::to_string(row));
	wstr.append("\t");

	//msg
	wstr.append(logstr);
	wstr.append("\n");

	//printf
#ifdef SYSTEM_WIN
	switch (level)
	{
	case LL_INFO:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |
			FOREGROUND_GREEN);
		break;
	case LL_WARNING:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |
			FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case LL_ERROR:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |
			FOREGROUND_RED);
		break;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	printf("%s", wstr.c_str());

	//ÉèÖÃ»Ø°×É«
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
	switch (level)
	{
	case L_INFO:
		printf("\033[1;32;40m%s\033[0m", wstr.c_str());
		break;
	case L_WARNING:
		printf("\033[1;33;40m%s\033[0m", wstr.c_str());
		break;
	case L_ERROR:
		printf("\033[1;31;40m%s\033[0m", wstr.c_str());
		break;
	default:
		printf("%s", wstr.c_str());
	}
#endif

	currsize += fprintf(file, "%s", wstr.c_str());
	fflush(file);
}


LogStream::LogStream(XLog * log, int level, const char * filename, const char * func, int line)
{
	_log = log;
	_level = level;
	_filename = filename;
	_func = func;
	_line = line;
}
LogStream::~LogStream()
{
	if (_log)
	{
		_log->writeLog(_level, _filename.c_str(), _func.c_str(), _line, stream.str().c_str());
		_log = NULL;
	}
}