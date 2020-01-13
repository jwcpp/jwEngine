import types.msg

struct testmsg
{
	ID id
	array arr
	player play
	vector<attr> attrs
	
	read{
		[id, arr, play, attrs]
	}
	write{
		[id, arr, play, attrs]
	}
}