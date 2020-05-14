#pragma once

#include "Define.h"

#ifdef SYSTEM_LINUX
typedef		int		SMHandle;
#else
typedef		HANDLE	SMHandle;
#endif

typedef unsigned long SM_KEY;

namespace ShareMemAPI
{
	SMHandle createShareMem(SM_KEY key, int size);

	SMHandle openShareMem(SM_KEY key, int size);

	char* mapShareMem(SMHandle handle);

	void unMapShareMem(char* memPtr);

	void closeShareMem(SMHandle handle);
};

