#include "Token.h"
#include "Keyword.h"
#include <assert.h>

static bool isNum(char c)
{
	return (c >= '0' && c <= '9');
}

static bool isLetter(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool isSpace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

static bool isVarChar(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_');
}


#include "Fio.h"

Token::Token(FBuffer * buffer)
{
	__m_pBuffer = buffer;
}


Token::~Token()
{
}

int Token::nextToken()
{
	__m_pBuffer->updateLast();
	__m_token = symbol();
	return __m_token;
}

char Token::next()
{
	return __m_pBuffer->next();
}

bool Token::check_next(char c)
{
	if (__m_pBuffer->current() == c) {
		next();
		return true;
	}
	return false;
}


int Token::read_var()
{
	const char * start_p = __m_pBuffer->currPointer();
	while (isVarChar(__m_pBuffer->current()))
	{
		next();
	}

	__m_name = std::string(start_p, __m_pBuffer->currPointer() - start_p);
	int tk = Keyword::getKeyword(__m_name.c_str());
	if (tk)
	{
		return tk;
	}
	else
	{
		return eKw_VAR;
	}
}

int Token::read_num()
{
	const char * start_p = __m_pBuffer->currPointer();
	bool isfloat = false;
	while (isNum(__m_pBuffer->current()) || __m_pBuffer->current() == '.')
	{
		if (__m_pBuffer->current() == '.')
			isfloat = true;
		next();
	}

	__m_name = std::string(start_p, __m_pBuffer->currPointer() - start_p);
	if (isfloat)
	{
		__m_f = atof(__m_name.c_str());
		return eKw_DECIMALS;
	}
	else
	{
		__m_i = atoi(__m_name.c_str());
		return eKw_NUM;
	}
}


int Token::symbol()
{
	
	for (;;)
	{
		char curr = __m_pBuffer->current();
		switch (curr)
		{
		case '\0':
			return eKw_EOF;

		case '\n': case '\r':
		{
			__m_pBuffer->nextline();
			break;
		}

		case ' ': case '\f': case '\t': case '\v':
		{
			next();
			break;
		}

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			return read_num();
		}

		case '/':
		{
			next();
			if (check_next('/'))
			{
				while (!__m_pBuffer->currIsNewline() && __m_pBuffer->current() != eKw_EOF)
					next();  /* skip until end of line (or end of file) */
				break;
			}

			if (check_next('*'))
			{
				read_long_string();
				break;
			}
			
			return '/';
		}

		case '=':
		{
			next();
			if (check_next('=')) return eKw_EQ;
			return '=';
		}
		case '!':
		{
			next();
			if (check_next('=')) return eKw_NE;
			return '!';
		}
		case '&':
		{
			next();
			if (check_next('&')) return eKw_AND;
			return '&';
		}
		case '|':
		{
			next();
			if (check_next('|')) return eKw_OR;
			return '|';
		}

		case '<':
		{
			next();
			if (check_next('=')) return eKw_LE;
			if (check_next('<')) return eKw_SHL;
			return '<';
		}

		case '>':
		{
			next();
			if (check_next('=')) return eKw_GE;
			if (check_next('>')) return eKw_SHR;
			return '>';
		}

		default:
			if (isLetter(curr))
			{
				return read_var();
			}
			next();
			return curr;
		}
	}

	return 0;
}


void Token::read_long_string()
{
	for (;;)
	{
		switch (__m_pBuffer->current())
		{
		case eKw_EOF:
			printf("多行注释未找到结尾\n");
			assert(0);
			break;

		case '\n':
		case '\r':
			__m_pBuffer->nextline();
			break;

		case '*':
		{
			next();
			if (check_next('/'))
			{
				return;
			}
		}

		default:
			next();
			break;
		}
	}
}