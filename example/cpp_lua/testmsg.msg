import types.msg

struct testmsg
{
	ID id
	Player play
	Array array
	vector<Attr> attrs
	vector<string> vstr
	
	read{
		[id, play, array, attrs, vstr]
	}
	write{
		[id, play, array, attrs, vstr]
	}
}