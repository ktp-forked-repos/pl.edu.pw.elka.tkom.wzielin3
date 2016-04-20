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
	deleteCurrentTokens();
}

void Lexer::deleteCurrentTokens()
{
	while (!tokens.empty())
	{
		delete tokens[tokens.size() - 1];
		tokens.pop_back();
	}
}

std::vector<LexerToken*> Lexer::scanText()
{
	deleteCurrentTokens();
	scanForWhitespace();
	unsigned int startPosition = currPosition;
	while (currPosition < toParse.size() && !isNextOpenTag()
			&& !isNextOpenSlashedTag())
	{
		currPosition++;
	}
	saveWordFrom(startPosition);
	scanForOpenSlashedTag() || scanForOpenTag();
	return tokens;
}

std::vector<LexerToken*> Lexer::scanTag()
{
	deleteCurrentTokens();
	unsigned int startPosition = currPosition;
	while (currPosition < toParse.size() && !isNextQuoteSign()
			&& !isNextCloseTag() && !isNextClosedSlashedTag())
	{
		if (!isNextWhitespace() && !isNextEqualSign())
		{
			currPosition++;
		}
		else
		{
			saveWordFrom(startPosition);
			scanForWhitespace();
			scanForEqualSign();
			startPosition = currPosition;
		}
	}
	saveWordFrom(startPosition);
	scanForQuoteSign() || scanForCloseTag() || scanForClosedSlashedTag();
	return tokens;
}

std::vector<LexerToken*> Lexer::scanQuotation()
{
	deleteCurrentTokens();
	unsigned int startPosition = currPosition;
	while (currPosition < toParse.size() && !isNextQuoteSign())
	{
		if(isNextEscapeSign() && currPosition + 1 < toParse.size())
		{
			currPosition += 2;
		}
		else if (!isNextWhitespace())
		{
			currPosition++;
		}
		else
		{
			saveWordFrom(startPosition);
			scanForWhitespace();
			startPosition = currPosition;
		}
	}
	saveWordFrom(startPosition);
	scanForQuoteSign();
	return tokens;
}

std::vector<LexerToken*> Lexer::scanScript()
{
	deleteCurrentTokens();
	unsigned int startPosition = currPosition;
	while(currPosition < toParse.size() && !isNextOpenSlashedTag() && !isNextQuoteSign())
	{
		currPosition++;
	}
	saveWordFrom(startPosition);
	scanForQuoteSign() || scanForOpenSlashedTag();
	return tokens;
}

void Lexer::saveWordFrom(unsigned int startPosition)
{
	if (startPosition < currPosition)
	{
		std::string result = toParse.substr(startPosition,
				currPosition - startPosition);
		tokens.push_back(new LexerToken(LexerTokenType::WORD, result));
	}
}

bool Lexer::isNextOpenTag()
{
	return toParse[currPosition] == OPEN_TAG
			&& (currPosition >= toParse.size()
					|| toParse[currPosition + 1] != FORWARD_SLASH);
}

bool Lexer::scanForOpenTag()
{
	if (isNextOpenTag())
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::OPEN_TAG));
		return true;
	}
	return false;
}

bool Lexer::isNextCloseTag()
{
	return toParse[currPosition] == CLOSE_TAG;
}

bool Lexer::scanForCloseTag()
{
	if (isNextCloseTag())
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::CLOSE_TAG));
		return true;
	}
	return false;
}

bool Lexer::isNextOpenSlashedTag()
{
	return toParse[currPosition] == OPEN_TAG
			&& currPosition + 1 < toParse.size()
			&& toParse[currPosition + 1] == FORWARD_SLASH;
}

bool Lexer::scanForOpenSlashedTag()
{
	if (isNextOpenSlashedTag())
	{
		currPosition += 2;
		tokens.push_back(new LexerToken(LexerTokenType::OPEN_SLASHED_TAG));
		return true;
	}
	return false;
}

bool Lexer::isNextClosedSlashedTag()
{
	if (toParse[currPosition] == FORWARD_SLASH)
	{
		if (currPosition + 1 < toParse.size()
				&& toParse[currPosition + 1] == CLOSE_TAG)
		{
			return true;
		}
	}
	return false;
}

bool Lexer::scanForClosedSlashedTag()
{
	if (isNextClosedSlashedTag())
	{
		currPosition += 2;
		tokens.push_back(new LexerToken(LexerTokenType::CLOSE_SLASHED_TAG));
		return true;
	}
	return false;
}

bool Lexer::isNextQuoteSign()
{
	return toParse[currPosition] == QUOTATION_MARK;
}

bool Lexer::scanForQuoteSign()
{
	if (isNextQuoteSign())
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::QUOTE_SIGN));
		return true;
	}
	return false;
}

bool Lexer::isNextEqualSign()
{
	return toParse[currPosition] == EQUAL_SIGN;
}

bool Lexer::scanForEqualSign()
{
	if (isNextEqualSign())
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::EQUAL_SIGN));
		return true;
	}
	return false;
}

bool Lexer::isNextWhitespace()
{
	return isspace((unsigned char)toParse[currPosition]);
}

bool Lexer::scanForWhitespace()
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

bool Lexer::isNextEscapeSign()
{
	if(currPosition < toParse.size() && toParse[currPosition] == ESCAPE_SIGN)
	{
		return true;
	}
	return false;
}

void Lexer::logError(std::string message)
{
	std::string error("Error in lexer at position ");

	std::stringstream ss;
	ss << currPosition;

	error += ss.str();
	error += ". ";
	error += message;
	ConsoleLog log;
	log.logError(error);
}
