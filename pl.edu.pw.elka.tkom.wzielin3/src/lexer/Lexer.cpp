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
	while (currPosition < toParse.size())
	{
		unsigned int previousPosition = currPosition;
		scanForOpenTag();
		scanForCloseTag();
		scanForOpenSlashedTag();
		scanForClosedSlashedTag();
		scanForEqualSign();
		scanForWhitespace();
		scanForWord();

		if (previousPosition == currPosition)
		{
			logError("Couldn't resolve symbol. Lexer stuck.");
		}
	}
	return tokens;
}

bool Lexer::isNextWord()
{
	bool isNot = isNextOpenTag() || isNextCloseTag() || isNextOpenSlashedTag()
			|| isNextClosedSlashedTag() || isNextQuoteSign()
			|| isNextEqualSign() || isNextWhitespace();
	return !isNot;
}

void Lexer::scanForWord()
{
	unsigned int startPosition = currPosition;
	while (currPosition < toParse.size() && isNextWord())
	{
		currPosition++;
	}
	if (startPosition != currPosition)
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

void Lexer::scanForOpenTag()
{
	if (isNextOpenTag())
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::OPEN_TAG));
	}
}

bool Lexer::isNextCloseTag()
{
	return toParse[currPosition] == CLOSE_TAG;
}

void Lexer::scanForCloseTag()
{
	if (isNextCloseTag())
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::CLOSE_TAG));
	}
}

bool Lexer::isNextOpenSlashedTag()
{
	return toParse[currPosition] == OPEN_TAG
			&& currPosition + 1 < toParse.size()
			&& toParse[currPosition + 1] == FORWARD_SLASH;
}

void Lexer::scanForOpenSlashedTag()
{
	if (isNextOpenSlashedTag())
	{
		currPosition += 2;
		tokens.push_back(new LexerToken(LexerTokenType::OPEN_SLASHED_TAG));
	}
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

void Lexer::scanForClosedSlashedTag()
{
	if (isNextClosedSlashedTag())
	{
		currPosition += 2;
		tokens.push_back(new LexerToken(LexerTokenType::CLOSE_SLASHED_TAG));
	}
}

bool Lexer::isNextQuoteSign()
{
	return toParse[currPosition] == QUOTATION_MARK;
}

void Lexer::scanForQuoteSign()
{
	if (isNextQuoteSign())
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::QUOTE_SIGN));
	}
}

bool Lexer::isNextEqualSign()
{
	return toParse[currPosition] == EQUAL_SIGN;
}

void Lexer::scanForEqualSign()
{
	if (isNextEqualSign())
	{
		currPosition++;
		tokens.push_back(new LexerToken(LexerTokenType::EQUAL_SIGN));
	}
}

bool Lexer::isNextWhitespace()
{
	return isspace(toParse[currPosition]);
}

void Lexer::scanForWhitespace()
{
	if (!isNextWhitespace())
	{
		return;
	}
	while (currPosition < toParse.size() && isNextWhitespace())
	{
		currPosition++;
	}
	tokens.push_back(new LexerToken(LexerTokenType::WHITESPACE));
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
