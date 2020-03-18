#pragma once

#include <string>
#include <vector>

class XFile
{
public:
	XFile();
	~XFile();

	static bool isExist(const char * dir);
	static int mkdir(const char * dir);
	// Delete empty directory
	static int rmdir(const char * dir);
	static int createDirectory(const char * dir);
	static int writeFile(const char * file, const char * content);
	static int readFile(const char * file, std::string & str);
	// Read by line
	static int readFile(const char *file, std::vector<std::string> & vec_str);
	static int remove(const char * file);
	// Walk through all the files in the directory
	static int listFiles(const char * dir, std::vector<std::string> & vec_file);
};

