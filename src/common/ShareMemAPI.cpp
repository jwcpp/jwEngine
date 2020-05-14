#include "ShareMemAPI.h"
#include <stdio.h>

#ifdef SYSTEM_LINUX
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

namespace ShareMemAPI
{


	SMHandle createShareMem(SM_KEY key, int size)
	{
#ifdef SYSTEM_LINUX
		SMHandle hd = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666);
		return hd;
#else
		WCHAR keybuf[sizeof(key)];
		memset(keybuf, 0, sizeof(key));
		swprintf(keybuf, L"%d", key);
		return  CreateFileMapping((HANDLE)0xFFFFFFFFFFFFFFFF, NULL, PAGE_READWRITE, 0, size, keybuf);
#endif
	}

	SMHandle openShareMem(SM_KEY key, int size)
	{
#ifdef SYSTEM_LINUX
		SMHandle hd = shmget(key, size, 0);
		return hd;
#else
		WCHAR keybuf[sizeof(key)];
		memset(keybuf, 0, sizeof(key));
		swprintf(keybuf, L"%d", key);
		return OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, keybuf);
#endif
	}

	char* mapShareMem(SMHandle handle)
	{
#ifdef SYSTEM_LINUX
		return  (char*)shmat(handle, 0, 0);
#else
		return (char *)MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#endif
		return 0;
	}


	void unMapShareMem(char* memPtr)
	{
#ifdef SYSTEM_LINUX
		shmdt(memPtr);
#else
		UnmapViewOfFile(memPtr);
#endif
	}


	void closeShareMem(SMHandle handle)
	{
#ifdef SYSTEM_LINUX
		shmctl(handle, IPC_RMID, 0);
#else
		CloseHandle(handle);
#endif
	}

}