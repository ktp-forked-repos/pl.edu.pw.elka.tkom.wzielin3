/*
 * LexerToken.h
 *
 *  Created on: Apr 7, 2016
 *      Author: wojciech
 */

#ifndef LEXER_LEXERTOKEN_H_
#define LEXER_LEXERTOKEN_H_

#include<string>

enum LexerTokenType
{
	OPEN_TAG,
	CLOSE_TAG,
	OPEN_SLASHED_TAG,
	CLOSE_SLASHED_TAG,
	BACKWARD_SLASH,
	FORWARD_SLASH,
	WORD,
	QUOTE_SIGN,
	EQUAL_SIGN,
	WHITESPACE
};

class LexerToken
{
public:
	LexerToken(LexerTokenType type);
	LexerToken(LexerTokenType type, std::string textContent);
	virtual ~LexerToken();

	LexerTokenType type;
	std::string textContent;
};

#endif /* LEXER_LEXERTOKEN_H_ */
