/*
 * HTLMParser.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "Parser.h"
#include<iostream>
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
{ LexerTokenType::EQUAL_SIGN, "=" } };

void Parser::parse()
{
	parseDoctypeDeclaration();
	parseDocument();
}

void Parser::parseDoctypeDeclaration()
{
	if (tokensAvailable(2) && currToken()->type == LexerTokenType::OPEN_TAG
			&& nextToken()->type == LexerTokenType::WORD
			&& nextToken()->getText() == "!DOCTYPE")
	{
		moveToNextToken();
		moveToNextToken();
		expectTokensAvailable();
		while (currToken()->type != LexerTokenType::CLOSE_TAG)
		{
			if (currToken()->type == LexerTokenType::WORD)
			{
				expectMoveToNextToken();
			}
			else if (currToken()->type == LexerTokenType::QUOTE_SIGN)
			{
				expectMoveToNextToken();
				while (currToken()->type == LexerTokenType::WORD)
				{
					expectMoveToNextToken();
				}
				expectTokenOfType(LexerTokenType::QUOTE_SIGN);
				expectMoveToNextToken();
			}
			else
			{
				logError(
						"Not expected token of type "
								+ tokenTypeToString.find(currToken()->type)->second
								+ " in !DOCTYPE declaration.");
			}
		}
		moveToNextToken();
	}
}

unsigned int Parser::parseDocument()
{
	while (tokensAvailable())
	{
		if (currToken()->type != LexerTokenType::OPEN_TAG
				&& currToken()->type != LexerTokenType::OPEN_SLASHED_TAG)
		{
			expectTokenOfType(LexerTokenType::WORD);
			HTMLElement* element = new HTMLElement();
			element->textContent = currToken()->getText();
			root->innerElements.push_back(element);
			moveToNextToken();
		}
		if (tokensAvailable()
				&& currToken()->type == LexerTokenType::OPEN_SLASHED_TAG)
		{
			expectMoveToNextToken();
			expectTokenOfType(LexerTokenType::WORD);
			expectMoveToNextToken();
			expectTokenOfType(LexerTokenType::CLOSE_TAG);
			moveToNextToken();
			return currPosition;
		}
		if (tokensAvailable())
		{
			expectTokenOfType(LexerTokenType::OPEN_TAG);
			HTMLElement* element = new HTMLElement();
			parseElement(element);
			root->innerElements.push_back(element);
		}
	}
	return currPosition;
}

void Parser::parseElement(HTMLElement* element)
{
	expectTokenOfType(LexerTokenType::OPEN_TAG);
	expectMoveToNextToken();
	expectTokenOfType(LexerTokenType::WORD);
	element->name = currToken()->getText();
	expectMoveToNextToken();
	while (true)
	{
		if (TryCloseCurrentElement(element->name))
		{
			return;
		}
		if (TryOpenCurrentElement(element->name))
		{
			Parser innerParser(tokens, currPosition, element);
			currPosition = innerParser.parseDocument();
			return;
		}
		HTMLAttribute* attr = new HTMLAttribute();
		parseAttribute(attr, element->name);
		element->attributes.push_back(attr);
	}
}

void Parser::parseAttribute(HTMLAttribute* attr, std::string currentElementName)
{
	expectTokenOfType(LexerTokenType::WORD);
	attr->name = currToken()->getText();
	expectMoveToNextToken();
	if (currToken()->type == LexerTokenType::EQUAL_SIGN)
	{
		expectMoveToNextToken();
		expectTokenOfType(LexerTokenType::QUOTE_SIGN);
		expectMoveToNextToken();
		while (currToken()->type != LexerTokenType::QUOTE_SIGN)
		{
			expectTokenOfType(LexerTokenType::WORD);
			attr->values.push_back(currToken()->getText());
			expectMoveToNextToken();
		}
		expectMoveToNextToken();
	}
}

bool Parser::TryCloseCurrentElement(std::string elementName)
{
	if (currToken()->type == CLOSE_SLASHED_TAG
			|| (currToken()->type == CLOSE_TAG
					&& selfClosingElements.find(elementName)
							!= selfClosingElements.end()))
	{
		moveToNextToken();
		return true;
	}
	return false;
}

bool Parser::TryOpenCurrentElement(std::string elementName)
{
	if (currToken()->type == LexerTokenType::CLOSE_TAG
			&& selfClosingElements.find(elementName)
					== selfClosingElements.end())
	{
		expectMoveToNextToken();
		return true;
	}
	return false;
}

bool Parser::expectTokensAvailable(unsigned count)
{
	if (!tokensAvailable(count))
	{
		logError("Expected more tokens. Input unexpectedly ended.");
		return false;
	}
	return true;
}

void Parser::expectTokenOfType(LexerTokenType type)
{
	if (currToken()->type != type)
	{
		std::string message = "Unexpected token. Expected ";
		message += tokenTypeToString.find(type)->second;
		message += ", but received ";
		message += tokenTypeToString.find(tokens[currPosition]->type)->second;
		message += ".";
		logError(message);
	}
}

void Parser::expectMoveToNextToken()
{
	moveToNextToken();
	expectTokensAvailable();
}

void Parser::moveToNextToken()
{
	currPosition++;
}

bool Parser::tokensAvailable(unsigned count)
{
	return currPosition + count <= tokens.size();
}

LexerToken* Parser::currToken()
{
	return tokens[currPosition];
}

LexerToken* Parser::nextToken()
{
	return tokens[currPosition + 1];
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
