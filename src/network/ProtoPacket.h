#pragma once

template<typename T, typename MSG>
void readProto(T* buff, MSG & msg)
{
	const char * data = buff->getBodyData();
	msg.ParseFromArray((const void*)data, buff->getBodySize());
}

template<typename T, typename MSG>
void writeProto(T* buff, MSG & msg)
{
	int size = msg.ByteSizeLong();
	buff->setWriteSize(size);
	const char* data = buff->getBodyData();
	msg.SerializeToArray((void *)data, size);
}