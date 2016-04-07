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
	while (!tokens.empty())
	{
		delete tokens[tokens.size() - 1];
		tokens.pop_back();
	}
}

const std::set<char> Lexer::specialCharacters =
{ '<', '>', '/', '"', '=', '\\' };

std::vector<LexerToken*> Lexer::scan()
{
	while (currPosition < toParse.size())
	{
		scanForOpenTag();
		scanForCloseTag();
		scanForOpenSlashedTag();
		scanForClosedSlashedTag();
		scanForWord();
		scanForQuoteSign();
		scanForEqualSign();
		scanForWhitespace();
		scanForBackwardSlash();
		scanForForwardSlash();
	}
	return tokens;
}

bool Lexer::scanForOpenTag()
{
	if (toParse[currPosition] == OPEN_TAG)
	{
		if (currPosition + 1 < toParse.size()
				&& toParse[currPosition + 1] != FORWARD_SLASH)
		{
			currPosition++;
		}
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::OPEN_TAG));
		return true;
	}
	return false;
}

bool Lexer::scanForCloseTag()
{
	if (toParse[currPosition] == CLOSE_TAG)
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::CLOSE_TAG));
		return true;
	}
	return false;
}

bool Lexer::scanForOpenSlashedTag()
{
	if (toParse[currPosition] == OPEN_TAG && currPosition + 1 < toParse.size()
			&& toParse[currPosition + 1] == FORWARD_SLASH)
	{
		currPosition += 2;
		tokens.push_back(new LexerToken(LexerTokenType::OPEN_SLASHED_TAG));
		return true;

	}
	return false;
}

bool Lexer::scanForClosedSlashedTag()
{
	if (toParse[currPosition] == FORWARD_SLASH)
	{
		if (currPosition + 1 < toParse.size()
				&& toParse[currPosition + 1] == CLOSE_TAG)
		{
			currPosition += 2;
			tokens.push_back(new LexerToken(LexerTokenType::CLOSE_SLASHED_TAG));
			return true;
		}
	}
	return false;
}

bool Lexer::scanForWord()
{
	std::string result;
	unsigned int startPosition = currPosition;
	while (currPosition < toParse.size()
			&& specialCharacters.find(toParse[currPosition])
					== specialCharacters.end()
			&& !isspace(toParse[currPosition]))
	{
		currPosition++;
	}
	result = toParse.substr(startPosition, currPosition - startPosition);
	if (result.size() > 0)
	{
		tokens.push_back(new LexerToken(LexerTokenType::WORD, result));
		return true;
	}
	return false;
}

bool Lexer::scanForQuoteSign()
{
	if (toParse[currPosition] == QUOTATION_MARK)
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::QUOTE_SIGN));
		return true;
	}
	return false;
}

bool Lexer::scanForEqualSign()
{
	if (toParse[currPosition] == EQUAL_SIGN)
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::EQUAL_SIGN));
		return true;
	}
	return false;
}

bool Lexer::scanForWhitespace()
{
	if (!isspace(toParse[currPosition]))
	{
		return false;
	}
	while (currPosition < toParse.size() && isspace(toParse[currPosition]))
	{
		currPosition++;
	}
	tokens.push_back(new LexerToken(LexerTokenType::WHITESPACE));
	return true;
}

bool Lexer::scanForBackwardSlash()
{
	if (toParse[currPosition] == BACKWARD_SLASH)
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::BACKWARD_SLASH));
		return true;
	}
	return false;
}

bool Lexer::scanForForwardSlash()
{
	if (toParse[currPosition] == FORWARD_SLASH)
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::FORWARD_SLASH));
		return true;
	}
	return false;
}
