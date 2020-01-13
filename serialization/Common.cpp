#include "Common.h"
#include "Token.h"
#include "Fio.h"
#include <assert.h>

Common::Common()
{
}


Common::~Common()
{
}

bool Common::testToken(Token * pToken, int token)
{
	if (pToken->getToken() == token)
	{
		pToken->nextToken();
		return true;
	}

	return false;
}

bool Common::checkToken(Token * pToken, int token)
{
	if (pToken->getToken() == token)
	{
		return true;
	}

	return false;
}

bool Common::matching(Token * pToken, int token)
{
	if (pToken->getToken() == token)
	{
		pToken->nextToken();
		return true;
	}
	
	error(pToken);
	return false;
}

int Common::error(Token * pToken)
{
	std::string errstr(pToken->getBuffer()->lastPointer(), pToken->getBuffer()->currPointer() - pToken->getBuffer()->lastPointer());
	printf("%s (%d) '%s' syntax error near.\n", pToken->getBuffer()->getFileName().c_str(), pToken->getBuffer()->getLine(), errstr.c_str());
	
	assert(0);
	return 0;
}