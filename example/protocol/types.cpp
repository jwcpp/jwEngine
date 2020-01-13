#include "types.h" 
#include "ByteBuffer.h" 
void player::read(ByteBuffer * buffer)
{
	*buffer >> name;
	*buffer >> level;
	if(level>=30)
	{
		*buffer >> msgcount;

	}
}
void player::write(ByteBuffer * buffer)
{
	*buffer << name;
	*buffer << level;
	if(level>=30)
	{
		*buffer << msgcount;

	}
}
void attr::read(ByteBuffer * buffer)
{
	*buffer >> attack;
	*buffer >> hp;
}
void attr::write(ByteBuffer * buffer)
{
	*buffer << attack;
	*buffer << hp;
}

