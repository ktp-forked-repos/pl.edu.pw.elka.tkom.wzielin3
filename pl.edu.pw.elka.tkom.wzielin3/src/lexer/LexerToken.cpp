/*
 * LexerToken.cpp
 *
 *  Created on: Apr 7, 2016
 *      Author: wojciech
 */

#include "LexerToken.h"



LexerToken::LexerToken(LexerTokenType type, std::string textContent) :
		type(type), textContent(textContent)
{
}

LexerToken::~LexerToken()
{
}

const std::map<LexerTokenType, std::string> LexerToken::tokenTypeToString =
{
{ LexerTokenType::OPEN_TAG, "<" },
{ LexerTokenType::CLOSE_TAG, ">" },
{ LexerTokenType::OPEN_SLASHED_TAG, "</" },
{ LexerTokenType::CLOSE_SLASHED_TAG, "/>" },
{ LexerTokenType::WORD, "" },
{ LexerTokenType::QUOTE_SIGN, "\"" },
{ LexerTokenType::EQUAL_SIGN, "=" } };

std::string LexerToken::getText()
{
	if (type == LexerTokenType::WORD)
	{
		return textContent;
	}
	return tokenTypeToString.find(type)->second;
}
bool LexerToken::isValid()
{
	return type != LexerTokenType::INVALID;
}

LexerToken LexerToken::getInvalid()
{
	return LexerToken(LexerTokenType::INVALID);
}
