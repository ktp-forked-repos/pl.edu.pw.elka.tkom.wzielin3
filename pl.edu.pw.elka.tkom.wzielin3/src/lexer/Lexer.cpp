/*
 * HTMLLexer.cpp
 *
 *  Created on: Apr 7, 2016
 *      Author: wojciech
 */

#include "Lexer.h"
#include <sstream>
#include "../log/ConsoleLog.h"

Lexer::Lexer(std::string toParse) :
		currPosition(0), toParse(toParse)
{
}

Lexer::~Lexer()
{
}

const std::string Lexer::OPEN_TAG = "<";
const std::string Lexer::OPEN_SLASHED_TAG = "</";
const std::string Lexer::CLOSE_TAG = ">";
const std::string Lexer::CLOSE_SLASHED_TAG = "/>";
const std::string Lexer::FORWARD_SLASH = "/";
const std::string Lexer::ESCAPE_SIGN = "\\";
const std::string Lexer::QUOTE_SIGN = "\"";
const std::string Lexer::EQUAL_SIGN = "=";
const std::string Lexer::OPEN_COMMENT = "<!--";
const std::string Lexer::CLOSE_COMMENT = "-->";
const std::string Lexer::DOCTYPE_OPEN = "<!DOCTYPE";

const std::list<std::pair<LexerTokenType, std::string> > Lexer::textScopeTokens =
{
{ LexerTokenType::OPEN_SLASHED_TAG, Lexer::OPEN_SLASHED_TAG },
{ LexerTokenType::OPEN_TAG, Lexer::OPEN_TAG } };

const std::list<std::pair<LexerTokenType, std::string> > Lexer::tagScopeTokens =
{
{ LexerTokenType::CLOSE_TAG, Lexer::CLOSE_TAG },
{ LexerTokenType::CLOSE_SLASHED_TAG, Lexer::CLOSE_SLASHED_TAG },
{ LexerTokenType::QUOTE_SIGN, Lexer::QUOTE_SIGN },
{ LexerTokenType::EQUAL_SIGN, Lexer::EQUAL_SIGN } };

const std::list<std::pair<LexerTokenType, std::string> > Lexer::htmlQuoteScopeTokens =
{
{ LexerTokenType::QUOTE_SIGN, Lexer::QUOTE_SIGN } };

const std::list<std::pair<LexerTokenType, std::string> > Lexer::scriptScopeTokens =
{
{ LexerTokenType::OPEN_SLASHED_TAG, Lexer::OPEN_SLASHED_TAG } };

bool Lexer::eof()
{
	return currPosition >= toParse.size();
}

LexerToken Lexer::scanScope(std::list<std::pair<LexerTokenType, std::string> > tokens, bool endOnWhitespace)
{
	unsigned int startPosition = currPosition;
	LexerToken token = LexerToken::getInvalid();
	while(currPosition < toParse.size() && !isNext(tokens)
			&& (!endOnWhitespace || !isNextWhitespace()))
	{
		currPosition++;
	}
	token = getWordFrom(startPosition);
	if(!token.isValid() && currPosition < toParse.size())
	{
		token = scanFor(tokens);
	}
	skipWhitespaces();
	return token;
}

LexerToken Lexer::scanFor(std::list<std::pair<LexerTokenType, std::string> > tokens)
{
	for(std::list<std::pair<LexerTokenType, std::string> >::iterator it = tokens.begin();
			it != tokens.end(); ++it)
	{
		if(scanFor(it->second))
		{
			return LexerToken(it->first);
		}
	}
	return LexerToken::getInvalid();
}

bool Lexer::scanFor(std::string word)
{
	if(isNext(word))
	{
		currPosition += word.size();
		return true;
	}
	return false;
}

bool Lexer::isNext(std::list<std::pair<LexerTokenType, std::string> > tokens)
{
	for(std::list<std::pair<LexerTokenType, std::string> >::iterator it = tokens.begin();
			it != tokens.end(); ++it)
	{
		if(isNext(it->second))
		{
			return true;
		}
	}
	return false;
}

bool Lexer::isNext(std::string word)
{
	return toParse.substr(currPosition, word.size()) == word;
}

LexerToken Lexer::scanText()
{
	skipWhitespacesAndComments();
	return scanScope(Lexer::textScopeTokens, false);
}

LexerToken Lexer::scanTag()
{
	skipWhitespaces();
	return scanScope(Lexer::tagScopeTokens, true);
}

LexerToken Lexer::scanHTMLQuote()
{
	return scanScope(Lexer::htmlQuoteScopeTokens, true);
}

LexerToken Lexer::scanScript()
{
	return scanScope(Lexer::scriptScopeTokens, false);
	unsigned int startPosition = currPosition;
	while(currPosition < toParse.size() && !isNext(Lexer::OPEN_SLASHED_TAG))
	{
		if (isNext(Lexer::QUOTE_SIGN))
		{
			currPosition++;
			skipScriptQuote();
		}
		currPosition++;
	}
	return getWordFrom(startPosition);
}

void Lexer::skipScriptQuote()
{
	while (currPosition < toParse.size() && !isNext(Lexer::QUOTE_SIGN))
	{
		if (isNext(Lexer::ESCAPE_SIGN) && currPosition + 1 < toParse.size())
		{
			currPosition += 2;
		}
		else
		{
			currPosition++;
		}
	}
}

void Lexer::skipDoctype()
{
	if(scanFor(Lexer::DOCTYPE_OPEN))
	{
		while(!scanFor(Lexer::CLOSE_TAG))
		{
			currPosition++;
		}
		currPosition++;
	}
}

LexerToken Lexer::getWordFrom(unsigned int startPosition)
{
	if (startPosition < currPosition)
	{
		std::string result = toParse.substr(startPosition,
				currPosition - startPosition);
		return LexerToken(LexerTokenType::WORD, result);
	}
	return LexerToken::getInvalid();
}

bool Lexer::isNextWhitespace()
{
	return isspace((unsigned char)toParse[currPosition]);
}

void Lexer::skipWhitespacesAndComments()
{
	while(skipWhitespaces() || skipComment());
}

bool Lexer::skipWhitespaces()
{
	if (!isNextWhitespace())
	{
		return false;
	}
	while (currPosition < toParse.size() && isNextWhitespace())
	{
		currPosition++;
	}
	return true;
}

bool Lexer::skipComment()
{
	if(currPosition  + 3 < toParse.size() && toParse.substr(currPosition, 4) == OPEN_COMMENT)
	{
		currPosition += 4;
		while(currPosition + 2 < toParse.size() && toParse.substr(currPosition, 3) != CLOSE_COMMENT)
		{
			currPosition++;
		}
		currPosition += 3;
		return true;
	}
	return false;
}

unsigned Lexer::getCurrentPosition()
{
	return currPosition;
}
