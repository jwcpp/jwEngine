#pragma once

class Token;
class Common
{
public:
	Common();
	~Common();

	static bool testToken(Token * pToken, int token);
	static bool checkToken(Token * pToken, int token);
	static bool matching(Token * pToken, int token);
	static int error(Token * pToken);
};

#define P_TOKEN pToken()

#define TEST_TOKEN(token) Common::testToken(P_TOKEN, token)
#define CHECK_TOKEN(token) Common::checkToken(P_TOKEN, token)
#define MATCH_TOKEN(token) Common::matching(P_TOKEN, token)
#define NEXT_TOKEN  P_TOKEN->nextToken()
#define TOKEN_ERROR Common::error(P_TOKEN)