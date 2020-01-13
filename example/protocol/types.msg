typedef uint32 ID
typedef vector<int32> array

struct player
{
	string name
	int32 level
	int32 msgcount
	
	read{
		[name, level]
		if(level >= 30)
		{
			[msgcount]
		}
	}
	write{
		[name, level]
		if(level >= 30)
		{
			[msgcount]
		}
	}
}

struct attr
{
	int32 attack
	int32 hp
	
	read{
		[attack, hp]
	}
	write{
		[attack, hp]
	}
}