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

std::vector<LexerToken*> Lexer::scan()
{
	scanText();
	return tokens;
}

void Lexer::scanText()
{
	unsigned int startPosition = currPosition;
	while (currPosition < toParse.size())
	{
		if (!isNextOpenTag() && !isNextClosedSlashedTag())
		{
			currPosition++;
		}
		else
		{
			saveWordFrom(startPosition);
			scanTag();
			startPosition = currPosition;
		}
	}
	saveWordFrom(startPosition);
}

void Lexer::scanTag()
{
	scanForOpenTag() || scanForOpenSlashedTag();
	unsigned int startPosition = currPosition;
	while (currPosition < toParse.size())
	{
		if (!isNextQuoteSign() && !isNextWhitespace() && !isNextEqualSign()
				&& !isNextCloseTag() && !isNextClosedSlashedTag())
		{
			currPosition++;
		}
		else
		{
			saveWordFrom(startPosition);
			if (isNextQuoteSign())
			{
				scanQuotation();
			}
			scanForWhitespace();
			scanForEqualSign();
			if (scanForCloseTag() || scanForClosedSlashedTag())
			{
				return;
			}
			startPosition = currPosition;
		}
	}
	saveWordFrom(startPosition);
}

void Lexer::scanQuotation()
{
	scanForQuoteSign();
	unsigned int startPosition = currPosition;
	while (currPosition < toParse.size())
	{
		if (!isNextQuoteSign() && !isNextWhitespace())
		{
			currPosition++;
		}
		else
		{
			saveWordFrom(startPosition);
			if (scanForQuoteSign())
			{
				return;
			}
			scanForWhitespace();
			startPosition = currPosition;
		}
	}
	saveWordFrom(startPosition);
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
	return isspace(toParse[currPosition]);
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
