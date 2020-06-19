#include <string_view>
#include "HttpConnect.h"
#include "BasePacket.h"
#include "http_parser.h"
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
name=zs&pass=111

-----------------
HTTP/1.1 200 OK\r\n
Content-Type: text/html\r\n
Content-Length: 10\r\n\r\n
data......
*/

http_parser_settings http_settings;
HttpConnect::HttpConnect()
{
	m_readPacket = new BasePacket();
	m_parser = (http_parser *)malloc(sizeof(http_parser));
	m_url = (http_parser_url*)malloc(sizeof(http_parser_url));
	m_parser->data = this;

	http_settings.on_message_begin = NULL;
	http_settings.on_url = &HttpConnect::on_url;
	http_settings.on_status = NULL;
	http_settings.on_header_field = NULL;
	http_settings.on_header_value = NULL;
	http_settings.on_headers_complete = NULL;
	http_settings.on_body = NULL;
	http_settings.on_message_complete = NULL;
	http_settings.on_chunk_header = NULL;
	http_settings.on_chunk_complete = NULL;

	http_parser_init(m_parser, HTTP_REQUEST);

	zero();
}

HttpConnect::~HttpConnect()
{
	delete m_readPacket;
	free(m_parser);
	free(m_url);
	release();
}

void HttpConnect::zero()
{
	m_readPacket->zero();
	m_residue = 0;
	m_urlp = NULL;
	m_content = NULL;
}

void HttpConnect::release()
{
	while (!m_writePackets.empty())
	{
		recyclePacket(m_writePackets.front());
		m_writePackets.pop();
	}
}

void HttpConnect::sendMsg(std::string_view sv)
{
	const char * p = sv.data();
	BasePacket * pack = createPacket();
	pack->append(p, sv.size());
	m_writePackets.push(pack);
	send_top_msg();
}

void HttpConnect::autoMsg(std::string_view sv, enum http_content_type type)
{
	static char buff[1024];
	sprintf(buff, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n", getContentTypeStr(type), sv.size());
	BasePacket * pack = createPacket();
	pack->append(buff, strlen(buff));
	pack->append((const uint8*)sv.data(), sv.size());
	m_writePackets.push(pack);
	send_top_msg();
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

BasePacket * HttpConnect::createPacket()
{
	BasePacket * packet = CommPool::create<BasePacket>();
	return packet;
}
void HttpConnect::recyclePacket(BasePacket * pack)
{
	CommPool::reclaim(pack);
}

int HttpConnect::on_url(http_parser* _, const char *at, size_t length)
{
	HttpConnect * conn = (HttpConnect *)(_->data);
	conn->m_urlp = at;
	return http_parser_parse_url(at, length, 0, conn->m_url);
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

				if (!parser((const char *)(m_readPacket->contents()), m_readPacket->wpos()))
				{
					close();
					break;
				}
				else
				{
					if (m_parser->method == HTTP_POST)
					{
						m_residue = m_parser->content_length;
					}
					m_close = (http_should_keep_alive(m_parser) == 0);
				}
			}
			else if (m_readPacket->wpos() >= 5120)
			{
				// Head is too big
				close();
				break;
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

bool HttpConnect::parser(const char * buf, int len)
{
	http_parser_execute(m_parser, &http_settings, buf, len);
	if (m_parser->http_errno != HPE_OK)
	{
		return false;
	}

	return true;
}

void HttpConnect::complete()
{
	std::string_view path(m_urlp + m_url->field_data[UF_PATH].off, m_url->field_data[UF_PATH].len);

	if (m_parser->method == HTTP_POST)
	{
		std::string_view param(m_content, m_parser->content_length);
		if (m_event) m_event->onMsg(this, HTTP_POST, path, param);
	}
	else if (m_parser->method == HTTP_GET)
	{
		std::string_view param;
		if ((m_url->field_set & (1 << UF_QUERY)) != 0)
		{
			param = std::string_view(m_urlp + m_url->field_data[UF_QUERY].off, m_url->field_data[UF_QUERY].len);
		}
		if (m_event) m_event->onMsg(this, HTTP_GET, path, param);
	}
	else
	{

	}
}

void HttpConnect::on_clsesocket()
{
	if(m_event) m_event->onClose(this);
}

void HttpConnect::on_writecomplete()
{
	if (m_close)
	{
		this->close();
	}
	else
	{
		if (m_writePackets.empty())
			return;

		recyclePacket(m_writePackets.front());
		m_writePackets.pop();
		send_top_msg();
	}
}

void HttpConnect::send_top_msg()
{
	if (m_writePackets.empty())
		return;

	BasePacket *pack = m_writePackets.front();
	write((char *)pack->contents(), pack->wpos());
}