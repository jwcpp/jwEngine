#pragma once 

#include "BaseType.h" 
#include <vector> 
class ByteBuffer;

typedef uint32 ID;
typedef std::vector<int32> array;
struct player
{
public:
	std::string name; 
	int32 level; 
	int32 msgcount; 
	void read(ByteBuffer * buffer);
	void write(ByteBuffer * buffer);
}; 
struct attr
{
public:
	int32 attack; 
	int32 hp; 
	void read(ByteBuffer * buffer);
	void write(ByteBuffer * buffer);
}; 

