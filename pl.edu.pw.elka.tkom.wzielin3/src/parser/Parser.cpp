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
	parseDocument();
}

void Parser::parseDocument()
{
	while (!lexer->eof())
	{
		token = lexer->scanText();
		if (token.type != LexerTokenType::OPEN_TAG && token.type != LexerTokenType::OPEN_SLASHED_TAG)
		{
			expectTokenOfType(LexerTokenType::WORD);
			HTMLElement* element = new HTMLElement();
			element->textContent = token.getText();
			root->innerElements.push_back(element);
			token = lexer->scanText();
			if(!token.isValid())
			{
				return;
			}
		}
		if (token.type == LexerTokenType::OPEN_SLASHED_TAG)
		{
			token = lexer->scanTag();
			expectTokenOfType(LexerTokenType::WORD);
			token = lexer->scanTag();
			expectTokenOfType(LexerTokenType::CLOSE_TAG);
			return;
		}
		else
		{
			expectTokenOfType(LexerTokenType::OPEN_TAG);
			HTMLElement* element = new HTMLElement();
			parseElement(element);
			root->innerElements.push_back(element);
		}
	}
}

void Parser::parseElement(HTMLElement* element)
{
	token = lexer->scanTag();
	expectTokenOfType(LexerTokenType::WORD);
	element->name = token.getText();
	if(equal(element->name, SCRIPT_TEXT))
	{
		parseScript(element);
		return;
	}
	token = lexer->scanTag();
	while (!TryCloseCurrentElement(element->name))
	{
		if (TryOpenCurrentElement(element->name))
		{
			Parser innerParser(lexer, element);
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
	token = lexer->scanTag();
	while (!TryCloseCurrentElement(element->name))
	{
		if (TryOpenCurrentElement(element->name))
		{
			token = lexer->scanScript();
			expectTokenOfType(LexerTokenType::WORD);
			HTMLElement* scriptContents = new HTMLElement();
			scriptContents->textContent = token.getText();
			element->innerElements.push_back(scriptContents);

			token = lexer->scanScript();
			expectTokenOfType(LexerTokenType::OPEN_SLASHED_TAG);
			token = lexer->scanTag();
			expectTokenOfType(LexerTokenType::WORD);
			token = lexer->scanTag();
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
	attr->name = token.getText();
	token = lexer->scanTag();
	expectTokenAvailable();
	if (token.type == LexerTokenType::EQUAL_SIGN)
	{
		token = lexer->scanTag();
		expectTokenOfType(LexerTokenType::QUOTE_SIGN);
		token = lexer->scanHTMLQuote();
		while (token.type != LexerTokenType::QUOTE_SIGN)
		{
			expectTokenOfType(LexerTokenType::WORD);
			attr->values.push_back(token.getText());
			token = lexer->scanHTMLQuote();
		}
		token = lexer->scanTag();
	}
}

bool Parser::TryCloseCurrentElement(std::string elementName)
{
	if (token.type == CLOSE_SLASHED_TAG
			|| (token.type == CLOSE_TAG
					&& selfClosingElements.find(elementName)
							!= selfClosingElements.end()))
	{
		return true;
	}
	return false;
}

bool Parser::TryOpenCurrentElement(std::string elementName)
{
	if (token.type == LexerTokenType::CLOSE_TAG
			&& selfClosingElements.find(elementName)
					== selfClosingElements.end())
	{
		return true;
	}
	return false;
}

void Parser::expectTokenOfType(LexerTokenType type)
{
	if (token.type != type)
	{
		std::string message = "Unexpected token. Expected ";
		message += tokenTypeToString.find(type)->second;
		message += ", but received ";
		message += token.getText();
		message += ".";
		logError(message);
	}
}

bool Parser::expectTokenAvailable()
{
	if (!lexer->eof())
	{
		return true;
	}
	logError("Expected more tokens. Input unexpectedly ended.");
	return false;
}

void Parser::logError(std::string message)
{
	std::string error("Error in parser");
	unsigned position = lexer->getCurrentPosition();

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
