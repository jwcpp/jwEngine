#include "XLog.h"
#include "XFile.h"
#include <time.h>
#include <iomanip>
#include <stdarg.h>
#include "Platform.h"
#include "XTime.h"

#ifdef SYSTEM_WIN
#include <windows.h>
#endif


XLog commonlog;

class XLog::AutoLock
{
public:
	AutoLock(SpinLock* lock) :_lock(lock)
	{
		_lock->lock();
	}
	~AutoLock()
	{ 
		_lock->unlock();
	}

	SpinLock* _lock;
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

void XLog::init(const char * rootdir, const char * child)
{
	logdir = rootdir;
	childdir = child;

	logdir.append("/");
	childdir.append("/");
	
	XFile::createDirectory(rootdir);
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
	const struct tm * tm_ = XTime::getTMStruct();

	{
		AutoLock templock(&_lock);

		bool newfile = (file == NULL);
		if (tm_->tm_mday != writeday)
		{
			writeday = tm_->tm_mday;
			char dirname[16];
			sprintf(dirname, "%d-%.2d-%.2d/", tm_->tm_year + 1900, tm_->tm_mon + 1, tm_->tm_mday);

			currdir = logdir;
			currdir.append(dirname);
			currdir.append(childdir);
			XFile::createDirectory(currdir.c_str());

			newfile = true;
		}

		if (newfile || currsize >= maxsize)
		{
			closefile();
			char dirname[16];
			sprintf(dirname, "%.2d.%.2d.%.2d.log", tm_->tm_hour, tm_->tm_min, tm_->tm_sec);
			std::string filepath = currdir;
			filepath.append(dirname);
			file = fopen(filepath.c_str(), "a+");
		}

		if (!file)return;
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

	print(level, wstr);

	AutoLock templock(&_lock);
	currsize += fprintf(file, "%s", wstr.c_str());
	fflush(file);
}

void XLog::print(int level, std::string& str)
{
	//printf
#ifdef SYSTEM_WIN
	{
		static SpinLock slock;
		AutoLock templock(&slock);
		switch (level)
		{
		case LL_WARNING:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |
				FOREGROUND_RED | FOREGROUND_GREEN);
			break;
		case LL_ERROR:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |
				FOREGROUND_RED);
			break;
		default:
			break;
		}

		printf("%s", str.c_str());

		//ÉèÖÃ»Ø°×É«
		if (level == LL_WARNING || level == LL_ERROR)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
	}
#else
	switch (level)
	{
	case LL_WARNING:
		printf("\033[1;33;40m%s\033[0m", str.c_str());
		break;
	case LL_ERROR:
		printf("\033[1;31;40m%s\033[0m", str.c_str());
		break;
	default:
		printf("%s", str.c_str());
	}
#endif
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