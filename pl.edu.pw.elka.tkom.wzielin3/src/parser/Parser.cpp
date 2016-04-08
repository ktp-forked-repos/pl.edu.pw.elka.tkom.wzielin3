/*
 * HTLMParser.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "Parser.h"

#include <sstream>
#include "../log/ConsoleLog.h"

Parser::Parser(std::vector<LexerToken*> tokens, int currentPosition,
		HTMLElement* root)
{
	this->tokens = tokens;
	this->currPosition = currentPosition;
	this->root = root;
}

Parser::~Parser()
{
}

const std::set<std::string> Parser::selfClosingElements =
{ "area", "base", "br", "col", "command", "embed", "hr", "img", "input",
		"keygen", "link", "meta", "param", "source", "track", "wbr" };

const std::map<LexerTokenType, std::string> Parser::tokenTypeToString =
{
{ LexerTokenType::OPEN_TAG, "<" },
{ LexerTokenType::CLOSE_TAG, ">" },
{ LexerTokenType::OPEN_SLASHED_TAG, "</" },
{ LexerTokenType::CLOSE_SLASHED_TAG, "/>" },
{ LexerTokenType::WORD, "WORD" },
{ LexerTokenType::QUOTE_SIGN, "\"" },
{ LexerTokenType::EQUAL_SIGN, "=" },
{ LexerTokenType::WHITESPACE, "WHITESPACE" } };

void Parser::parse()
{
	parseDoctypeDeclaration();
	parseDocument();
}

void Parser::parseDoctypeDeclaration()
{
	if (currentTokenIsOfType(LexerTokenType::OPEN_TAG)
			&& currPosition + 1 < tokens.size()
			&& tokens[currPosition + 1]->type == LexerTokenType::WORD
			&& tokens[currPosition + 1]->getText() == "!DOCTYPE")
	{
		currPosition += 2;
		while (currPosition < tokens.size()
				&& !currentTokenIsOfType(LexerTokenType::CLOSE_TAG))
		{
			parseWhiteSpace();
			if (currentTokenIsOfType(LexerTokenType::WORD))
			{
				currPosition++;
				parseWhiteSpace();
			}
			else if (currentTokenIsOfType(LexerTokenType::QUOTE_SIGN))
			{
				currPosition++;
				parseWhiteSpace();
				while (!currentTokenIsOfType(LexerTokenType::QUOTE_SIGN))
				{
					parseQuotedWord();
					parseWhiteSpace();
				}
				currPosition++;
			}
			else
			{
				logError("Unexpected error while parsing !DOCTYPE declaration");
			}
		}
	}
}

unsigned int Parser::parseDocument()
{
	parseWhiteSpace();
	while (currPosition < tokens.size())
	{
		if (currentTokenIsOfType(LexerTokenType::OPEN_SLASHED_TAG))
		{
			CloseElement();
			return currPosition;
		}
		HTMLElement* element = new HTMLElement();
		root->innerElements.push_back(element);
		if (currentTokenIsOfType(LexerTokenType::OPEN_TAG))
		{
			parseElement(element);
		}
		else
		{
			parseText(element);
		}
		parseWhiteSpace();
	}
	return currPosition;
}

void Parser::parseElement(HTMLElement* element)
{
	currPosition++;
	parseWhiteSpace();
	expectTokenOfType(LexerTokenType::WORD);
	element->name = tokens[currPosition]->getText();
	parseWhiteSpace();
	while (!TryCloseCurrentElement(element->name))
	{
		if (TryOpenCurrentElement(element->name))
		{
			Parser innerParser(tokens, currPosition, element);
			currPosition = innerParser.parseDocument();
			break;
		}
		HTMLAttribute* attr = new HTMLAttribute();
		parseAttribute(attr, element->name);
		element->attributes.push_back(attr);

		parseWhiteSpace();
	}
}

void Parser::parseText(HTMLElement* element)
{
	element->textContent = "";
	while (currPosition < tokens.size()
			&& !currentTokenIsOfType(OPEN_SLASHED_TAG)
			&& !currentTokenIsOfType(OPEN_TAG))
	{
		element->textContent += tokens[currPosition]->getText();
		currPosition++;
	}
}

void Parser::parseAttribute(HTMLAttribute* attr, std::string currentElementName)
{
	expectTokenOfType(LexerTokenType::WORD);
	attr->name = tokens[currPosition]->getText();
	currPosition++;
	parseWhiteSpace();
	if (currentTokenIsOfType(LexerTokenType::EQUAL_SIGN))
	{
		currPosition++;
		parseWhiteSpace();
		expectTokenOfType(LexerTokenType::QUOTE_SIGN);
		currPosition++;
		parseWhiteSpace();
		while (!currentTokenIsOfType(LexerTokenType::QUOTE_SIGN))
		{
			std::string value = parseQuotedWord();
			attr->values.push_back(value);
			parseWhiteSpace();
		}
		currPosition++;
	}
}

std::string Parser::parseQuotedWord()
{
	std::string result;
	while (!currentTokenIsOfType(LexerTokenType::QUOTE_SIGN)
			&& !currentTokenIsOfType(LexerTokenType::WHITESPACE))
	{
		result += tokens[currPosition]->getText();
		currPosition++;
	}
	return result;
}

void Parser::parseWhiteSpace()
{
	if (currPosition < tokens.size()
			&& tokens[currPosition]->type == LexerTokenType::WHITESPACE)
	{
		currPosition++;
	}
}

void Parser::CloseElement()
{
	while (currPosition < tokens.size()
			&& !currentTokenIsOfType(LexerTokenType::CLOSE_SLASHED_TAG)
			&& !currentTokenIsOfType(LexerTokenType::CLOSE_TAG))
	{
		currPosition++;
	}
	if (currPosition < tokens.size())
	{
		currPosition++;
	}
}

bool Parser::TryCloseCurrentElement(std::string elementName)
{
	if (tokens[currPosition]->type == CLOSE_SLASHED_TAG
			|| (tokens[currPosition]->type == CLOSE_TAG
					&& selfClosingElements.find(elementName)
							!= selfClosingElements.end()))
	{
		currPosition++;
		return true;
	}
	return false;
}

bool Parser::TryOpenCurrentElement(std::string elementName)
{
	if (currentTokenIsOfType(LexerTokenType::CLOSE_TAG)
			&& selfClosingElements.find(elementName)
					== selfClosingElements.end())
	{
		currPosition++;
		return true;
	}
	return false;
}

void Parser::expectMoreTokens()
{
	if (currPosition >= tokens.size())
	{
		logError("Expected more tokens. Input unexpectedly ended.");
	}
}

void Parser::expectTokenOfType(LexerTokenType type)
{
	if (!currentTokenIsOfType(type))
	{
		std::string message = "Unexpected token. Expected ";
		message += tokenTypeToString.find(type)->second;
		message += ", but received ";
		message += tokenTypeToString.find(tokens[currPosition]->type)->second;
		message += ".";
		logError(message);
	}
}

bool Parser::currentTokenIsOfType(LexerTokenType type)
{
	expectMoreTokens();
	return tokens[currPosition]->type == type;
}

void Parser::logError(std::string message)
{
	std::string error("Error in parser at position ");

	std::stringstream ss;
	ss << currPosition;

	error += ss.str();
	error += ". ";
	error += message;
	ConsoleLog log;
	log.logError(error);
}
