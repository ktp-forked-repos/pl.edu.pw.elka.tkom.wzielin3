/*
 * HTLMParser.h
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#ifndef PARSER_HTMLPARSER_H_
#define PARSER_HTMLPARSER_H_

#include<vector>
#include<set>
#include<string>
#include"HTMLElement.h"

class HTMLParser
{
public:
	HTMLParser();
	virtual ~HTMLParser();

	HTMLElement* parse(std::string toParse);
private:
	std::string toParse;
	unsigned int currPosition;

	static const char OPEN_TAG = '<';
	static const char CLOSE_TAG = '>';
	static const char CLOSE_SLASH = '/';

	static const std::set<std::string> selfClosingElements;

	HTMLElement* parseElement();
	HTMLElement* parseText();
	HTMLAttribute* parseAttribute();

	bool isElementSelfClosing(std::string elementName)
	{
		return selfClosingElements.find(elementName)
				!= selfClosingElements.end();
	}
};
#endif /* PARSER_HTMLPARSER_H_ */
