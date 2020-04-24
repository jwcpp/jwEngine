#include "XFile.h"
#include <string>
#include <fstream>
#include <sstream> 
#include "Define.h"

#ifdef SYSTEM_WIN
	#include <io.h>
	#include <direct.h>
#else
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <dirent.h>
#endif

XFile::XFile()
{
}


XFile::~XFile()
{
}


bool XFile::isExist(const char * dir)
{
#ifdef SYSTEM_WIN
	return _access(dir, 0) == 0;
#else
	return access(dir, 0) == 0;
#endif
}

int XFile::mkdir(const char * dir)
{
#ifdef SYSTEM_WIN
	return _mkdir(dir);
#else
	return ::mkdir(dir, 0755);
#endif
}

int XFile::rmdir(const char * dir)
{
#ifdef SYSTEM_WIN
	return _rmdir(dir);
#else
	return rmdir(dir);
#endif
}

int XFile::createDirectory(const char * dir)
{
	std::string path = dir;
	int len = path.length();
	char tmpDirPath[256] = { 0 };
	for (int i = 0; i < len; i++)
	{
		tmpDirPath[i] = path[i];
		if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/')
		{
			if (!isExist(tmpDirPath))
			{
				int ret = mkdir(tmpDirPath);
				if (ret == -1) return ret;
			}
		}
	}
	return 0;
}

int XFile::writeFile(const char * file, const char * content)
{
	std::ofstream fout(file);
	if (fout.is_open())
	{
		fout << content << std::endl;
		fout.close();
		return 0;
	}
	return -1;
}

int XFile::readFile(const char * file, std::string & str)
{
	std::ifstream in(file);
	if (in.is_open())
	{
		std::stringstream buffer;
		buffer << in.rdbuf();
		str = buffer.str();
	}

	return -1;
}

int XFile::readFile(const char *file, std::vector<std::string> & vec_str)
{
	std::ifstream ifile(file);
	if (ifile.is_open())
	{
		int row = 0;
		while (ifile.good())
		{
			std::string line;
			getline(ifile, line);
			vec_str.push_back(line);
		}
		ifile.close();
		return 0;
	}

	return -1;
}

int XFile::remove(const char * file)
{
	return ::remove(file);
}

int XFile::listFiles(const char * dir, std::vector<std::string> & vec_file)
{
	std::string folderPath = dir;
#ifdef SYSTEM_WIN
	_finddata_t FileInfo;
	std::string strfind = folderPath + "\\*";
	long Handle = _findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		return -1;
	}
	do {
		//判断是否有子目录
		if (FileInfo.attrib & _A_SUBDIR)
		{
			//这个语句很重要
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
			{
				std::string newPath = folderPath + "\\" + FileInfo.name;
				listFiles(newPath.c_str(), vec_file);
			}
		}
		else
		{
			std::string filename = (folderPath + "\\" + FileInfo.name);
			vec_file.push_back(filename);
		}
	} while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);

#else
	struct stat s;
	stat(folderPath.c_str(), &s);
	if (!S_ISDIR(s.st_mode)) {
		return false;
	}
	DIR* open_dir = opendir(folderPath.c_str());
	if (NULL == open_dir) {
		std::exit(EXIT_FAILURE);
	}
	dirent* p = nullptr;
	while ((p = readdir(open_dir)) != nullptr) {
		struct stat st;
		if (p->d_name[0] != '.') {
			//因为是使用devC++ 获取windows下的文件，所以使用了 "\" ,linux下要换成"/"
			std::string name = folderPath + std::string("\\") + std::string(p->d_name);
			stat(name.c_str(), &st);
			if (S_ISDIR(st.st_mode)) {
				listFiles(name.c_str(), vec_file);
			}
			else if (S_ISREG(st.st_mode)) {
				vec_file.push_back(name);
			}
		}
	}
	closedir(open_dir);
#endif

	return 0;
}