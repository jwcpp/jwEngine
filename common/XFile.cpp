#include "XFile.h"
#include <io.h>
#include <direct.h>
#include <string>

XFile::XFile()
{
}


XFile::~XFile()
{
}


bool XFile::existDir(const char * dir)
{
	return _access(dir, 0) == 0;
}

int XFile::mkdir(const char * dir)
{
	return _mkdir(dir);
}


int XFile::createDirectory(const char * dir)
{
	std::string path = dir;
	int len = path.length();
	char tmpDirPath[256] = { 0 };
	for (int i = 0; i < len; i++)
	{
		tmpDirPath[i] = path[i];
		if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/' || path[i + 1] == '\0')
		{
			if (!existDir(tmpDirPath))
			{
				int ret = mkdir(tmpDirPath);
				if (ret == -1) return ret;
			}
		}
	}
	return 0;
}