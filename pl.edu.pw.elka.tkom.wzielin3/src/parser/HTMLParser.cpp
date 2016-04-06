/*
 * HTLMParser.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "HTMLParser.h"

HTMLParser::HTMLParser(std::string toParse, int currentPosition,
		HTMLElement* root)
{
	this->toParse = toParse;
	this->currPosition = currentPosition;
	this->root = root;
}

HTMLParser::~HTMLParser()
{
	// TODO Auto-generated destructor stub
}

const std::set<std::string> HTMLParser::selfClosingElements =
{ "area", "base", "br", "col", "command", "embed", "hr", "img", "input", "keygen", "link", "meta", "param", "source", "track", "wbr" };

const std::set<std::string> HTMLParser::ignoredElements =
{ "!DOCTYPE" };

unsigned int HTMLParser::parse()
{
	while (currPosition < toParse.size())
	{
		parseWhiteSpaces();
		if (IsNextCloseOpenedElement())
		{
			CloseOpenedElement();
			return currPosition;
		}
		HTMLElement* element = new HTMLElement();
		root->innerElements.push_back(element);
		if (IsNextNewCurrentElement())
		{
			parseElement(element);
		}
		else
		{
			parseText(element);
		}
	}
	return currPosition;
}

void HTMLParser::parseElement(HTMLElement* element)
{
	currPosition++;
	element->name = parseWord();
	parseWhiteSpaces();
	if(ignoredElements.find(element->name) != ignoredElements.end())
	{
		CloseOpenedElement();
		return;
	}
	while (!TryCloseCurrentElement(element->name))
	{
		if (TryOpenCurrentElement(element->name))
		{
			HTMLParser innerParser(toParse, currPosition, element);
			currPosition = innerParser.parse();
			break;
		}
		HTMLAttribute* attr = new HTMLAttribute();
		parseAttribute(attr, element->name);
		element->attributes.push_back(attr);

		parseWhiteSpaces();
	}
}

void HTMLParser::parseText(HTMLElement* element)
{
	int startPosition = currPosition;
	while (currPosition < toParse.size() && !IsNextNewCurrentElement()
			&& !IsNextCloseOpenedElement())
	{
		currPosition++;
	}
	element->textContent = toParse.substr(startPosition,
			currPosition - startPosition);
	return;
}

void HTMLParser::parseAttribute(HTMLAttribute* attr,
		std::string currentElementName)
{
	attr->name = parseWord();
	parseWhiteSpaces();
	if (toParse[currPosition] == EQUAL_SIGN)
	{
		currPosition++;
		parseWhiteSpaces();
		if (toParse[currPosition] != QUOTATION_MARK)
		{
			throw "Unexpeted character when parsing attribute - exepcted '\"'.";
		}
		currPosition++;
		while (toParse[currPosition] != QUOTATION_MARK)
		{
			parseWhiteSpaces();
			std::string value = parseQuotedWord();
			attr->values.push_back(value);
			parseWhiteSpaces();
		}
		currPosition++;
	}
}

std::string HTMLParser::parseWord()
{
	int startPosition = currPosition;
	while (currPosition < toParse.size() && !isspace(toParse[currPosition]) && !isSpecialCharacter(toParse[currPosition]))
	{
		currPosition++;
	}
	return toParse.substr(startPosition, currPosition - startPosition);
}

std::string HTMLParser::parseQuotedWord()
{
	int startPosition = currPosition;
	while (currPosition < toParse.size() && !isspace(toParse[currPosition]) && !(toParse[currPosition] == QUOTATION_MARK))
	{
		currPosition++;
	}
	return toParse.substr(startPosition, currPosition - startPosition);
}

void HTMLParser::parseWhiteSpaces()
{
	while(isspace(toParse[currPosition]))
	{
		currPosition++;
	}
}


bool HTMLParser::IsNextCloseOpenedElement()
{
	bool closeRoot = toParse[currPosition] == OPEN_TAG
			&& toParse[currPosition + 1] == CLOSE_SLASH;
	return closeRoot;
}

void HTMLParser::CloseOpenedElement()
{
	while (toParse[currPosition] != CLOSE_TAG)
	{
		currPosition++;
	}
	currPosition++;
}

bool HTMLParser::IsNextNewCurrentElement()
{
	return toParse[currPosition] == OPEN_TAG
			&& toParse[currPosition + 1] != CLOSE_SLASH;
}

bool HTMLParser::TryCloseCurrentElement(std::string elementName)
{
	if (toParse[currPosition] == CLOSE_SLASH
			&& toParse[currPosition + 1] == CLOSE_TAG)
	{
		currPosition += 2;
		return true;
	}
	if (toParse[currPosition] == CLOSE_TAG
			&& selfClosingElements.find(elementName)
					!= selfClosingElements.end())
	{
		currPosition++;
		return true;
	}
	return false;
}

bool HTMLParser::TryOpenCurrentElement(std::string elementName)
{
	bool openCurrent = toParse[currPosition] == CLOSE_TAG
			&& selfClosingElements.find(elementName)
					== selfClosingElements.end();
	if (openCurrent)
	{
		currPosition++;
		return true;
	}
	return false;
}

bool HTMLParser::isSpecialCharacter(char c)
{
	return c == EQUAL_SIGN || c == CLOSE_SLASH
		|| c == CLOSE_TAG || c == OPEN_TAG
		|| c == QUOTATION_MARK;
}
