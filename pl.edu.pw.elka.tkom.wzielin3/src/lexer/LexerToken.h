/*
 * LexerToken.h
 *
 *  Created on: Apr 7, 2016
 *      Author: wojciech
 */

#ifndef LEXER_LEXERTOKEN_H_
#define LEXER_LEXERTOKEN_H_

#include<string>
#include<map>

enum LexerTokenType
{
	OPEN_TAG,
	CLOSE_TAG,
	OPEN_SLASHED_TAG,
	CLOSE_SLASHED_TAG,
	WORD,
	QUOTE_SIGN,
	EQUAL_SIGN
};

class LexerToken
{
public:
	LexerToken(LexerTokenType type, std::string textContent = "");
	virtual ~LexerToken();


	LexerTokenType type;
	std::string getText();
private:
	std::string textContent;
	static const std::map<LexerTokenType, std::string> tokenTypeToString;
};

#endif /* LEXER_LEXERTOKEN_H_ */
