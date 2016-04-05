/*
 * HTLMParser.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "HTMLParser.h"

HTMLParser::HTMLParser()
{
}

HTMLParser::~HTMLParser()
{
	// TODO Auto-generated destructor stub
}

const std::set<std::string> HTMLParser::selfClosingElements
{ "area", "base", "br", "col", "command", "embed", "hr", "img", "input",
		"keygen", "link", "meta", "param", "source", "track", "wbr" };

HTMLElement* HTMLParser::parse(std::string toParse)
{
	this->toParse = toParse;
	this->currPosition = 0;

	if (this->toParse.size() == 0)
	{
		throw "String to parse cannot be of length 0.";
	}

	HTMLElement* rootElement = new HTMLElement();
	while (currPosition < toParse.size())
	{
		HTMLElement* element;
		if (toParse[currPosition] == OPEN_TAG)
		{
			element = parseElement();
		}
		else
		{
			element = parseText();
		}
		rootElement->innerElements.push_back(element);
	}
	return rootElement;
}

HTMLElement* HTMLParser::parseElement()
{
	//TODO implement
	throw "Not implemented";
}

HTMLElement* HTMLParser::parseText()
{
	//TODO implement
	throw "Not implemented";
}

HTMLAttribute* HTMLParser::parseAttribute()
{
	//TODO implement
	throw "Not implemented";
}
