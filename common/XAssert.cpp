#include "XAssert.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void IAssert(char const* file, int line, char const* function, char const* message, char const* format, ...)
{
	va_list args;
	va_start(args, format);

	fprintf(stderr, "\n%s:%i in %s ASSERTION FAILED:\n  %s ", file, line, function, message);
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	fflush(stderr);

	va_end(args);
	*((volatile int*)NULL) = 0;
	exit(1);
}