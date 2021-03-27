#include <string_view>
#include "HttpConnect.h"
#include "BasePacket.h"
#include "CommonPool.h"

/*
-----------------
GET /login?name=zs&pass=111 HTTP/1.1\r\n
Host: 127.0.0.1\r\n
Connection: keep-alive\r\n\r\n

-----------------
POST /login HTTP/1.1\r\n
Host: 127.0.0.1\r\n
Connection: keep-alive\r\n
Content-Length:10\r\n
\r\n
name=zs&pass=111

-----------------
HTTP/1.1 200 OK\r\n
Content-Type: text/html\r\n
Content-Length: 10\r\n\r\n
data......
*/

HttpConnect::HttpConnect()
{
	m_readPacket = createPacket();
	zero();
}

HttpConnect::~HttpConnect()
{
	recyclePacket(m_readPacket);
}

void HttpConnect::zero()
{
	TcpSocket::zero();
	m_readPacket->zero();
	m_parser.zero();
	m_residue = 0;
	m_content = NULL;
}

void HttpConnect::sendMsg(const char* msg, int32 len)
{
	BasePacket* pack = createPacket();
	pack->append(msg, len);
	TcpSocket::write(pack);
}
void HttpConnect::sendData(std::string_view sv)
{
	sendMsg(sv.data(), sv.size());
}

void HttpConnect::autoMsg(std::string_view sv, enum http_content_type type)
{
	static char buff[1024];
	sprintf(buff, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n", getContentTypeStr(type), sv.size());
	BasePacket * pack = createPacket();
	pack->append(buff, strlen(buff));
	pack->append((const uint8*)sv.data(), sv.size());
	TcpSocket::write(pack);
}

const char * HttpConnect::getContentTypeStr(enum http_content_type type)
{
	const char *ctype = "text/html";
	switch (type)
	{
	case hct_text_html:		ctype = "text/html"; break;
	case hct_text_plain:	ctype = "text/plain"; break;
	case hct_text_xml:		ctype = "text/xml"; break;
	case hct_text_json:		ctype = "text/json"; break;
	case hct_image_gif:		ctype = "image/gif"; break;
	case hct_image_jpeg:	ctype = "image/jpeg"; break;
	case hct_image_png:		ctype = "image/png"; break;
	default:
		break;
	}

	return ctype;
}

void HttpConnect::on_msgbuffer(MessageBuffer * buffer)
{
	// http head end: \r\n\r\n
	while (buffer->GetActiveSize() > 0)
	{
		// find head end pos
		if (m_content == NULL)
		{
			std::string_view view((char *)buffer->GetReadPointer());
			int rpos = view.find("\r\n\r\n");
			if (rpos != std::string_view::npos)
			{
				m_readPacket->append(buffer->GetReadPointer(), rpos + 4);
				buffer->ReadCompleted(rpos + 4);
				m_content = (const char *)(m_readPacket->contents() + m_readPacket->wpos());

				if (!m_parser.parser((const char *)(m_readPacket->contents()), m_readPacket->wpos()))
				{
					close();
					break;
				}
				else
				{
					if (m_parser.method() == HTTP_POST)
					{
						m_residue = static_cast<int>(m_parser.contentLen());
					}
				}
			}
			else
			{
				m_readPacket->append(buffer->GetReadPointer(), buffer->GetActiveSize());
				buffer->ReadCompleted(buffer->GetActiveSize());

				if (m_readPacket->wpos() >= 0xffff)
				{
					// Head is too big
					close();
					break;
				}
			}
		}
		
		if(m_content)
		{
			int rlen = buffer->GetActiveSize();
			if (m_residue > 0)
			{
				rlen = m_residue > rlen ? rlen : m_residue;
				m_residue -= rlen;
				m_readPacket->append(buffer->GetReadPointer(), rlen);
				buffer->ReadCompleted(rlen);
			}

			if (m_residue == 0)
			{
				complete();
				zero();
			}
		}
	}
}

void HttpConnect::complete()
{
	std::string_view path = m_parser.getUrl()->getPath();

	if (m_parser.method() == HTTP_POST)
	{
		std::string_view param(m_content, static_cast<int>(m_parser.contentLen()));
		if (m_event) m_event->onPost(this, path, param);
	}
	else if (m_parser.method() == HTTP_GET)
	{
		std::string_view param;
		if (m_parser.getUrl()->haveParam())
		{
			param = m_parser.getUrl()->getParam();
		}
		if (m_event) m_event->onGet(this, path, param);
	}
	else
	{
		if (m_event) m_event->onOther(this, &m_parser);
	}
}

void HttpConnect::on_clsesocket()
{
	if(m_event) m_event->onClose(this);
}

void HttpConnect::on_writecomplete()
{
	if (m_parser.isClose())
	{
		this->close();
	}

	TcpSocket::on_writecomplete();
}