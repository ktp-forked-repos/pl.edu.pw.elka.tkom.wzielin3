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

const std::set<std::string> Parser::ignoredElements =
{ "!DOCTYPE" };

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

unsigned int Parser::parse()
{
	parseWhiteSpaces();
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
		parseWhiteSpaces();
	}
	return currPosition;
}

void Parser::parseElement(HTMLElement* element)
{
	currPosition++;
	expectTokenOfType(LexerTokenType::WORD);
	element->name = tokens[currPosition]->getText();
	parseWhiteSpaces();
	if (ignoredElements.find(element->name) != ignoredElements.end())
	{
		CloseElement();
		return;
	}
	while (!TryCloseCurrentElement(element->name))
	{
		if (TryOpenCurrentElement(element->name))
		{
			Parser innerParser(tokens, currPosition, element);
			currPosition = innerParser.parse();
			break;
		}
		HTMLAttribute* attr = new HTMLAttribute();
		parseAttribute(attr, element->name);
		element->attributes.push_back(attr);

		parseWhiteSpaces();
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
	parseWhiteSpaces();
	if (currentTokenIsOfType(LexerTokenType::EQUAL_SIGN))
	{
		currPosition++;
		parseWhiteSpaces();
		expectTokenOfType(LexerTokenType::QUOTE_SIGN);
		currPosition++;
		while (!currentTokenIsOfType(LexerTokenType::QUOTE_SIGN))
		{
			parseWhiteSpaces();
			expectTokenOfType(LexerTokenType::WORD);
			std::string value = tokens[currPosition]->getText();
			attr->values.push_back(value);
			currPosition++;
			parseWhiteSpaces();
		}
		currPosition++;
	}
}

void Parser::parseWhiteSpaces()
{
	while (currPosition < tokens.size()
			&& tokens[currPosition]->type == LexerTokenType::WHITESPACE)
	{
		currPosition++;
	}
}

void Parser::CloseElement()
{
	while (!currentTokenIsOfType(LexerTokenType::CLOSE_SLASHED_TAG)
			&& !currentTokenIsOfType(LexerTokenType::CLOSE_TAG))
	{
		currPosition++;
	}
	currPosition++;
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
		message +=", but received ";
		message += tokenTypeToString.find(tokens[currPosition]->type)->second;
		message += ".";
		logError(message);
	}
}

bool Parser::previousTokenIsOfType(LexerTokenType type)
{
	return tokens[currPosition - 1]->type == type;
}

bool Parser::currentTokenIsOfType(LexerTokenType type)
{
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
