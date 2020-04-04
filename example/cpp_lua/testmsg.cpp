#include "testmsg.h" 
#include "ByteBuffer.h" 
bool testmsg::read(ByteBuffer * buffer)
{
	if(sizeof(id) + buffer->rpos() > buffer->wpos()) return false;
	*buffer >> id;
	if(play.read(buffer) == false) return false;
	int len_array;
	if(sizeof(len_array) + buffer->rpos() > buffer->wpos()) return false;
	*buffer >> len_array;
	for( int i = 0; i< len_array; ++i) { 
		int32 temp_array;
		if(sizeof(temp_array) + buffer->rpos() > buffer->wpos()) return false;
		*buffer >> temp_array;
		array.push_back(temp_array);
	}
	int len_attrs;
	if(sizeof(len_attrs) + buffer->rpos() > buffer->wpos()) return false;
	*buffer >> len_attrs;
	for( int i = 0; i< len_attrs; ++i) { 
		Attr temp_attrs;
		if(temp_attrs.read(buffer) == false) return false;
		attrs.push_back(temp_attrs);
	}
	int len_vstr;
	if(sizeof(len_vstr) + buffer->rpos() > buffer->wpos()) return false;
	*buffer >> len_vstr;
	for( int i = 0; i< len_vstr; ++i) { 
		std::string temp_vstr;
		*buffer >> temp_vstr;
		vstr.push_back(temp_vstr);
	}
	return true;
}
void testmsg::write(ByteBuffer * buffer)
{
	*buffer << id;
	play.write(buffer);
	int len_array = array.size();
	*buffer << len_array;
	for( int i = 0; i< len_array; ++i) { 
		*buffer << array[i];
	}
	int len_attrs = attrs.size();
	*buffer << len_attrs;
	for( int i = 0; i< len_attrs; ++i) { 
		attrs[i].write(buffer);
	}
	int len_vstr = vstr.size();
	*buffer << len_vstr;
	for( int i = 0; i< len_vstr; ++i) { 
		*buffer << vstr[i];
	}
}

