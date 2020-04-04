#pragma once 

#include "BaseType.h" 
#include <vector> 
class ByteBuffer;

typedef uint32 ID;
typedef std::vector<int32> Array;
struct Player
{
public:
	std::string name; 
	int32 level; 
	int32 msgcount; 
	bool read(ByteBuffer * buffer);
	void write(ByteBuffer * buffer);
}; 
struct Attr
{
public:
	int32 attack; 
	int32 hp; 
	bool read(ByteBuffer * buffer);
	void write(ByteBuffer * buffer);
}; 

