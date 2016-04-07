/*
 * LexerToken.cpp
 *
 *  Created on: Apr 7, 2016
 *      Author: wojciech
 */

#include "LexerToken.h"

LexerToken::LexerToken(LexerTokenType type) : LexerToken(type, "")
{

}

LexerToken::LexerToken(LexerTokenType type, std::string textContent) :
		type(type), textContent(textContent)
{
}

LexerToken::~LexerToken()
{
}

