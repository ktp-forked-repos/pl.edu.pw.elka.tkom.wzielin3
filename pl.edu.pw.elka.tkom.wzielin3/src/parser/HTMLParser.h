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
	HTMLParser(std::string toParse, int currentPosition, HTMLElement* root);
	virtual ~HTMLParser();

	unsigned int parse();
private:
	HTMLElement* root;
	std::string toParse;
	unsigned int currPosition;

	static const char OPEN_TAG = '<';
	static const char CLOSE_TAG = '>';
	static const char CLOSE_SLASH = '/';
	static const char QUOTATION_MARK = '"';
	static const char EQUAL_SIGN = '=';
	static const std::set<std::string> selfClosingElements;

	void parseElement(HTMLElement* element);
	void parseText(HTMLElement* element);
	void parseAttribute(HTMLAttribute* attr, std::string currentElementName);
	std::string parseWord();
	std::string parseQuotedWord();
	void parseWhiteSpaces();

	//encountered </
	bool IsNextCloseOpenedElement();
	void CloseOpenedElement();
	//encountered <
	bool IsNextNewCurrentElement();
	//encountered /> or element is self closing
	bool TryCloseCurrentElement(std::string elementName);
	//encountered > and element is not self closing
	bool TryOpenCurrentElement(std::string elementName);

	bool isSpecialCharacter(char c);
};
#endif /* PARSER_HTMLPARSER_H_ */
