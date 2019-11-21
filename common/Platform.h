#ifndef PLATFORM_H
#define PLATFORM_H
/**
 * 平台相关
 */
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define SYSTEM_WIN // win 平台
#else
#define SYSTEM_LINUX // linux 平台
#endif


#endif


