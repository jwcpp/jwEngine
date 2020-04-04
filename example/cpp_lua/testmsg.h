#pragma once 

#include "BaseType.h" 
#include <vector> 
class ByteBuffer;

#include "types.h"
struct testmsg
{
public:
	ID id; 
	Player play; 
	Array array; 
	std::vector<Attr> attrs; 
	std::vector<std::string> vstr; 
	bool read(ByteBuffer * buffer);
	void write(ByteBuffer * buffer);
}; 

