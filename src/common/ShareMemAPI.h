#pragma once

#if defined(SYSTEM_LINUX)
typedef		int		SMHandle;
#else
typedef		void*	SMHandle;
#endif

//typedef	ULONG	SM_KEY;
typedef unsigned long SM_KEY;

namespace ShareMemAPI
{
	/*创建ShareMem 内存区
	*
	*	key   创建ShareMem 的关键值
	*
	*  Size  创建大小
	*
	*	返回 对应ShareMem保持值
	*/
	SMHandle		CreateShareMem(SM_KEY key, int Size);
	/*打开ShareMem 内存区
	*
	* key   打开ShareMem 的关键值
	*
	* Size  打开大小
	*
	* 返回  对应ShareMem 保持值
	*/
	SMHandle		OpenShareMem(SM_KEY key, int Size);

	/*映射ShareMem 内存区
	*
	*	handle 映射ShareMem 的保持值
	*
	*  返回 ShareMem 的数据指针
	*/
	char*			MapShareMem(SMHandle handle);

	/*关闭映射 ShareMem 内存区
	*
	*	MemoryPtr			ShareMem 的数据指针
	*
	*
	*/
	void			UnMapShareMem(char* MemoryPtr);

	/*	关闭ShareMem
	* 	handle  需要关闭的ShareMem 保持值
	*/
	void			CloseShareMem(SMHandle handle);
};

