#include "testmsg.h" 
#include "ByteBuffer.h" 
void testmsg::read(ByteBuffer * buffer)
{
	*buffer >> id;
	int len_arr = 0;
	*buffer >> len_arr;
	for( int i = 0; i< len_arr; ++i) { 
		int32 temp_arr;
		*buffer >> temp_arr;
		arr.push_back(temp_arr);
	}
	play.read(buffer);
	int len_attrs = 0;
	*buffer >> len_attrs;
	for( int i = 0; i< len_attrs; ++i) { 
		attr temp_attrs;
		temp_attrs.read(buffer);
		attrs.push_back(temp_attrs);
	}
}
void testmsg::write(ByteBuffer * buffer)
{
	*buffer << id;
	int len_arr = arr.size();
	*buffer << len_arr; 
	for( int i = 0; i< len_arr; ++i) { 
		*buffer << arr[i];
	}
	play.write(buffer);
	int len_attrs = attrs.size();
	*buffer << len_attrs; 
	for( int i = 0; i< len_attrs; ++i) { 
		attrs[i].write(buffer);
	}
}

