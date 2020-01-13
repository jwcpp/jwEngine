#pragma once 

#include "BaseType.h" 
#include <vector> 
class ByteBuffer;

#include "types.h"
struct testmsg
{
public:
	ID id; 
	array arr; 
	player play; 
	std::vector<attr> attrs; 
	void read(ByteBuffer * buffer);
	void write(ByteBuffer * buffer);
}; 

