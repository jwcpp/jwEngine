#include "ShareMemAPI.h"
#include <stdio.h>

#ifdef SYSTEM_WIN
#include <WinBase.h>
#else
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <errno.h>
#endif

namespace ShareMemAPI
{


	SMHandle		CreateShareMem(SM_KEY key, UINT Size)
	{
			//CHAR keybuf[64];
			//memset(keybuf,0,64);
			//sprintf(keybuf,"./%d",key);
#ifdef SYSTEM_LINUX
			//key = ftok(keybuf,'w'); 
			SMHandle hd = shmget(key, Size, IPC_CREAT | IPC_EXCL | 0666);
		printf("handle = %d ,key = %d ,error: %d \r\n", hd, key, errno);
		return hd;
#else SYSTEM_WIN
		WCHAR keybuf[64];
		memset(keybuf, 0, 64);
		swprintf(keybuf, L"%d", key);
		return  CreateFileMapping((HANDLE)0xFFFFFFFFFFFFFFFF, NULL, PAGE_READWRITE, 0, Size, keybuf);
#endif
			return SMHandle(-1);

	}

	SMHandle		OpenShareMem(SM_KEY key, UINT Size)
	{
			//CHAR keybuf[64];
			//memset(keybuf,0,64);
			//sprintf(keybuf,"./%d",key);
#ifdef SYSTEM_LINUX
			//key = ftok(keybuf,'w'); 
			SMHandle hd = shmget(key, Size, 0);
		printf("handle = %d ,key = %d ,error: %d \r\n", hd, key, errno);
		return hd;
#else SYSTEM_WIN
		WCHAR keybuf[64];
		memset(keybuf, 0, 64);
		swprintf(keybuf, L"%d", key);
		return OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, keybuf);
#endif
			return SMHandle(-1);
	}

	CHAR*			MapShareMem(SMHandle handle)
	{

#ifdef SYSTEM_LINUX
			return  (CHAR*)shmat(handle, 0, 0);
#else SYSTEM_WIN
			return (CHAR *)MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#endif
			return 0;

	}


	VOID			UnMapShareMem(CHAR* MemoryPtr)
	{
#ifdef SYSTEM_LINUX
			shmdt(MemoryPtr);
#else SYSTEM_WIN
			UnmapViewOfFile(MemoryPtr);
#endif
	}


	VOID			CloseShareMem(SMHandle handle)
	{
#ifdef SYSTEM_LINUX
			shmctl(handle, IPC_RMID, 0);
#else SYSTEM_WIN
			CloseHandle(handle);
#endif
	}

}