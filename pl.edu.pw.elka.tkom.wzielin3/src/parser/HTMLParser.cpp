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

const std::set<std::string> HTMLParser::selfClosingElements
{ "area", "base", "br", "col", "command", "embed", "hr", "img", "input",
		"keygen", "link", "meta", "param", "source", "track", "wbr" };

void HTMLParser::parse()
{
	while (currPosition < toParse.size())
	{
		if (IsNextCloseOpenedElement())
		{
			CloseOpenedElement();
			return;
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
}

void HTMLParser::parseElement(HTMLElement* element)
{
	currPosition++;
	element->name = parseWord();
	parseWhiteSpaces();
	while (!TryCloseCurrentElement(element->name))
	{
		if (TryOpenCurrentElement(element->name))
		{
			HTMLParser innerParser(toParse, currPosition, element);
			innerParser.parse();
			break;
		}
		HTMLAttribute* attr;
		parseAttribute(attr, element->name);
		element->attributes.push_back(attr);

		parseWhiteSpaces();
	}
	return;
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
			std::string value = parseWord();
			attr->value.push_back(value);
			parseWhiteSpaces();
		}
		currPosition++;
	}
}

std::string HTMLParser::parseWord()
{
	int startPosition = currPosition;
	while (currPosition < toParse.size() && !isspace(toParse[currPosition]))
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
					== selfClosingElements.end())
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
					!= selfClosingElements.end();
	if (openCurrent)
	{
		currPosition++;
		return true;
	}
	return false;
}
