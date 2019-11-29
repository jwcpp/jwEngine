#pragma once

class XFile
{
public:
	XFile();
	~XFile();

	static bool existDir(const char * dir);
	static int mkdir(const char * dir);
	static int createDirectory(const char * dir);
};

