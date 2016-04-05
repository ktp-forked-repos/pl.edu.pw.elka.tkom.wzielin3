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
	/**
	 * @param toParse string which should be parsed
	 * @param currentPoisition index in the toParse string from which parse should start
	 * @param root HTMLElement to which parser will append parsed elements
	 */
	HTMLParser(std::string toParse, int currentPosition, HTMLElement* root);
	virtual ~HTMLParser();

	/**
	 * Method that starts parsing process.
	 * @returns position in the string where parsing was finished and from which may be continued outside this parser.
	 */
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

	/**
	 * Method that extracts HTMLElement, that may have nested elements, from current position.
	 * Requires that a start tag at current position (<).
	 * If no start tag is found at current position consider using parseText.
	 * @param element HTMLElement where parsed data is saved
	 */
	void parseElement(HTMLElement* element);

	/**
	 * Method that extracts HTMLElement, that has no nestes elements, from current position.
	 * Shoudl be used only for plain text in the document.
	 * If current position start with start tag (<) use parseElement insteaad.
	 * @param element HTMLElement where parsed data is saved
	 */
	void parseText(HTMLElement* element);

	/**
	 * Method that extracts Attribute from current position for HTMLElement.
	 * Use when current position is inside HTMLElement and begins with attribute name.
	 * @param attr HTMLAttrbiute to which parsed data is saved
	 * @param currentElementName name of an element that contains extracted attribute
	 */
	void parseAttribute(HTMLAttribute* attr, std::string currentElementName);

	/**
	 * Method that extracts a word from current position. Shoudl be used for words that
	 * are not in quotation marks, i.e. attribute names, element names.
	 * @returns std::string that begins in current posistion and
	 * ends with either a whitespace or any html special character.
	 */
	std::string parseWord();

	/**
	 * Method that extracts a word from current position. Should be used for words that
	 * are in quoatation marks, i.e. attribute values.
	 * @returns std::string that begins in current position and
	 * ends with either a whitespace or a quotation mark.
	 */
	std::string parseQuotedWord();

	/**
	 * Method used for skipping whitespaces. Increments current position as long as
	 * current positiong is a whitespace.
	 */
	void parseWhiteSpaces();

	/**
	 * Method that checks whether next characters indicate that current root element is to be closed.
	 * It detects </ so tags like </div>, </html> etc.
	 * @returns true if current root element is ending.
	 */
	bool IsNextCloseOpenedElement();

	/**
	 * Method that skips closing tag of an element. It will increment current position of parser
	 * until it reaches > character. Consider using it when IsNextCloseOpenedElement returns true.
	 */
	void CloseOpenedElement();

	/**
	 * Method that detects opening tag <.
	 * @returns true when next characters belong to a new element.
	 */
	bool IsNextNewCurrentElement();

	/**
	 * When parsing an opening tag of an element it detects whether next characters
	 * indicate that an opening tag is also a closing tag, in other words that
	 * the current element has no contents. Detects /> or > when element is self closing.
	 * After detecting closing characters it incremenets current position to skip those characters.
	 * @param elementName name of the current HTMLElement.
	 * @returns true if the element has been closed. False if nothing has been done and element is not closed.
	 */
	bool TryCloseCurrentElement(std::string elementName);

	/**
	 * When parsing an opening tag of an element it detects whether next characters
	 * indicate that an opening tag is closing and that element has contents, in other
	 * word that current element has a seperate closing tag and may have nested elements.
	 * Detects > when element is not self closing.
	 * @param elementName name of the current element
	 * @returns true if current element has been opened and it's content may be parsed. False if
	 * nothing has been done and opening tag should be parsed further.
	 */
	bool TryOpenCurrentElement(std::string elementName);

	/**
	 * Checks if character is a special HTML character.
	 * @param c character to be checked
	 * @returns true if c is a special character.
	 */
	bool isSpecialCharacter(char c);
};
#endif /* PARSER_HTMLPARSER_H_ */
