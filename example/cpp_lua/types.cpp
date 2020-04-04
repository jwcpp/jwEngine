#include "types.h" 
#include "ByteBuffer.h" 
bool Player::read(ByteBuffer * buffer)
{
	*buffer >> name;
	if(sizeof(level) + buffer->rpos() > buffer->wpos()) return false;
	*buffer >> level;
	if(level>=30)
	{
		if(sizeof(msgcount) + buffer->rpos() > buffer->wpos()) return false;
		*buffer >> msgcount;

	}
	return true;
}
void Player::write(ByteBuffer * buffer)
{
	*buffer << name;
	*buffer << level;
	if(level>=30)
	{
		*buffer << msgcount;

	}
}
bool Attr::read(ByteBuffer * buffer)
{
	if(sizeof(attack) + buffer->rpos() > buffer->wpos()) return false;
	*buffer >> attack;
	if(sizeof(hp) + buffer->rpos() > buffer->wpos()) return false;
	*buffer >> hp;
	return true;
}
void Attr::write(ByteBuffer * buffer)
{
	*buffer << attack;
	*buffer << hp;
}

