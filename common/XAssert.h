#pragma once

void IAssert(char const* file, int line, char const* function, char const* message, char const* format, ...);

#define XAssert(cond, ...) do { if (!(cond)) IAssert(__FILE__, __LINE__, __FUNCTION__, #cond, ##__VA_ARGS__); } while(0)