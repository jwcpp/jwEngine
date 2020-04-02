#include <string>
#include "DB_Interface.h"

DB_Interface::DB_Interface(const char * ip, unsigned int port):
	m_ip(ip),
	m_port(port)
{

}

DB_Interface::~DB_Interface()
{

}