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

Parser::Parser(Lexer* lexer, HTMLElement* root)
{
	this->lexer = lexer;
	this->currPosition = 0;
	this->root = root;
}

Parser::~Parser()
{
}

const std::set<std::string> Parser::selfClosingElements =
{ "area", "base", "br", "col", "command", "embed", "hr", "img", "input",
		"keygen", "link", "meta", "param", "source", "track", "wbr", "!DOCTYPE" };

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
	lexerScanText();
	if (currToken()->type != LexerTokenType::OPEN_TAG)
	{
		return;
	}
	lexerScanTag();
	if (currToken()->type == LexerTokenType::WORD
			&& currToken()->getText() == "!DOCTYPE")
	{
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
				lexerScanQuotation();
				while (currToken()->type == LexerTokenType::WORD)
				{
					expectMoveToNextToken();
				}
				expectTokenOfType(LexerTokenType::QUOTE_SIGN);
				lexerScanTag();
			}
			else
			{
				logError(
						"Not expected token of type "
								+ tokenTypeToString.find(currToken()->type)->second
								+ " in !DOCTYPE declaration.");
			}
		}
	}
}

unsigned int Parser::parseDocument()
{
	lexerScanText();
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
			lexerScanTag();
			expectTokenOfType(LexerTokenType::WORD);
			expectMoveToNextToken();
			expectTokenOfType(LexerTokenType::CLOSE_TAG);
			return currPosition;
		}
		if (tokensAvailable())
		{
			expectTokenOfType(LexerTokenType::OPEN_TAG);
			HTMLElement* element = new HTMLElement();
			parseElement(element);
			root->innerElements.push_back(element);
			lexerScanText();
		}
	}
	return currPosition;
}

void Parser::parseElement(HTMLElement* element)
{
	lexerScanTag();
	expectTokenOfType(LexerTokenType::WORD);
	element->name = currToken()->getText();
	if(element->name == "script")
	{
		parseScript(element);
		return;
	}
	expectMoveToNextToken();
	while (true)
	{
		if (TryCloseCurrentElement(element->name))
		{
			return;
		}

		if (TryOpenCurrentElement(element->name))
		{
			Parser innerParser(lexer, element);
			innerParser.parseDocument();
			return;
		}
		HTMLAttribute* attr = new HTMLAttribute();
		parseAttribute(attr, element->name);
		element->attributes.push_back(attr);
		lexerScanTag();
	}
}


void Parser::parseScript(HTMLElement* element)
{
	expectMoveToNextToken();
	while (true)
	{
		if (TryCloseCurrentElement(element->name))
		{
			return;
		}

		if (TryOpenCurrentElement(element->name))
		{
			lexerScanScript();
		}
		HTMLAttribute* attr = new HTMLAttribute();
		parseAttribute(attr, element->name);
		element->attributes.push_back(attr);
		lexerScanTag();
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
		lexerScanQuotation();
		while (currToken()->type != LexerTokenType::QUOTE_SIGN)
		{
			expectTokenOfType(LexerTokenType::WORD);
			std::string s = currToken()->getText();
			attr->values.push_back(currToken()->getText());
			expectMoveToNextToken();
		}
	}
}

bool Parser::TryCloseCurrentElement(std::string elementName)
{
	if (currToken()->type == CLOSE_SLASHED_TAG
			|| (currToken()->type == CLOSE_TAG
					&& selfClosingElements.find(elementName)
							!= selfClosingElements.end()))
	{
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

void Parser::lexerScanText()
{
	tokens = lexer->scanText();
	currPosition = 0;
}

void Parser::lexerScanTag()
{
	tokens = lexer->scanTag();
	currPosition = 0;
}

void Parser::lexerScanQuotation()
{
	tokens = lexer->scanQuotation();
	currPosition = 0;
}

void Parser::lexerScanScript()
{
	tokens = lexer->scanScript();
	currPosition = 0;
}

void Parser::logError(std::string message)
{
	std::string error("Error in parser");

	error += ". ";
	error += message;
	ConsoleLog log;
	log.logError(error);
}
