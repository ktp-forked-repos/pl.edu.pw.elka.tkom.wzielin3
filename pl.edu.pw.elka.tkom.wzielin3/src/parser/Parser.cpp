/*
 * HTLMParser.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "Parser.h"
#include <algorithm>
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

const std::string Parser::SCRIPT_TEXT = "script";

void Parser::parse()
{
	lexer->skipDoctype();
	lexerScanText();
	parseDocument();
}

void Parser::parseDocument()
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
			if(!tokensAvailable())
			{
				return;
			}
			expectMoveToNextToken();
		}
		if (currToken()->type == LexerTokenType::OPEN_SLASHED_TAG)
		{
			lexerScanTag();
			expectTokenOfType(LexerTokenType::WORD);
			expectMoveToNextToken();
			expectTokenOfType(LexerTokenType::CLOSE_TAG);
			return;
		}
		else
		{
			expectTokenOfType(LexerTokenType::OPEN_TAG);
			HTMLElement* element = new HTMLElement();
			lexerScanTag();
			parseElement(element);
			root->innerElements.push_back(element);
			lexerScanText();
		}
	}
}

void Parser::parseElement(HTMLElement* element)
{
	expectTokenOfType(LexerTokenType::WORD);
	element->name = currToken()->getText();
	if(equal(element->name, SCRIPT_TEXT))
	{
		parseScript(element);
		return;
	}
	expectMoveToNextToken();
	while (!TryCloseCurrentElement(element->name))
	{
		if (TryOpenCurrentElement(element->name))
		{
			Parser innerParser(lexer, element);
			innerParser.lexerScanText();
			innerParser.parseDocument();
			return;
		}
		HTMLAttribute* attr = new HTMLAttribute();
		parseAttribute(attr, element->name);
		element->attributes.push_back(attr);
	}
}


void Parser::parseScript(HTMLElement* element)
{
	expectMoveToNextToken();
	while (!TryCloseCurrentElement(element->name))
	{
		if (TryOpenCurrentElement(element->name))
		{
			lexerScanScript(); 
			expectTokenOfType(LexerTokenType::WORD);
			HTMLElement* scriptContents = new HTMLElement();
			scriptContents->textContent = currToken()->getText();
			element->innerElements.push_back(scriptContents);

			lexerScanTag();
			expectTokenOfType(LexerTokenType::WORD);
			expectMoveToNextToken();
			expectTokenOfType(LexerTokenType::CLOSE_TAG);
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
		lexerScanHTMLQuote();
		while (currToken()->type != LexerTokenType::QUOTE_SIGN)
		{
			expectTokenOfType(LexerTokenType::WORD);
			attr->values.push_back(currToken()->getText());
			expectMoveToNextToken();
		}
		lexerScanTag();
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

void Parser::expectTokenOfType(LexerTokenType type)
{
	if (currToken()->type != type)
	{
		std::string message = "Unexpected token. Expected ";
		message += tokenTypeToString.find(type)->second;
		message += ", but received ";
		message += tokens[currPosition]->getText();
		message += ".";
		logError(message);
	}
}

void Parser::expectMoveToNextToken()
{
	currPosition++;
	if (!tokensAvailable())
	{
		logError("Expected more tokens. Input unexpectedly ended.");
	}
}

bool Parser::expectTokensAvailable()
{
	if (tokensAvailable())
	{
		return true;
	}
	logError("Expected more tokens. Input unexpectedly ended.");
	return false;
}

bool Parser::tokensAvailable()
{
	return currPosition + 1 <= tokens.size();
}

LexerToken* Parser::currToken()
{
	return tokens[currPosition];
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

void Parser::lexerScanHTMLQuote()
{
	tokens = lexer->scanHTMLQuote();
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

bool Parser::equal(std::string a, std::string b)
{
	std::transform(a.begin(), a.end(), a.begin(), ::tolower);
	std::transform(b.begin(), b.end(), b.begin(), ::tolower);
	return a == b;
}
