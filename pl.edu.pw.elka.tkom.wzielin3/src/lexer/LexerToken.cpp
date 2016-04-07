/*
 * LexerToken.cpp
 *
 *  Created on: Apr 7, 2016
 *      Author: wojciech
 */

#include "LexerToken.h"

LexerToken::LexerToken(LexerTokenType type) :
		LexerToken(type, "")
{

}

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
{ LexerTokenType::BACKWARD_SLASH, "\\" },
{ LexerTokenType::FORWARD_SLASH, "/" },
{ LexerTokenType::WORD, "" },
{ LexerTokenType::QUOTE_SIGN, "\"" },
{ LexerTokenType::EQUAL_SIGN, "=" },
{ LexerTokenType::WHITESPACE, " " } };

std::string LexerToken::getText()
{
	if (type == LexerTokenType::WORD)
	{
		return textContent;
	}
	else
		return tokenTypeToString.find(type)->second;
}
