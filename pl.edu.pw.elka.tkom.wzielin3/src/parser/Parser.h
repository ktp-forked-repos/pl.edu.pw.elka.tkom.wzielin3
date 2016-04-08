/*
 * HTLMParser.h
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include<vector>
#include<set>
#include<string>
#include"HTMLElement.h"
#include"../lexer/LexerToken.h"

class Parser
{
public:
	/**
	 * @param tokens to be parsed
	 * @param currentPoisition index in the toParse string from which parse should start
	 * @param root HTMLElement to which parser will append parsed elements
	 */
	Parser(std::vector<LexerToken*> tokens, int currentPosition, HTMLElement* root);
	virtual ~Parser();

	/**
	 * Method that carries out parsing process.
	 */
	void parse();
private:
	HTMLElement* root;
	std::vector<LexerToken*> tokens;
	unsigned int currPosition;

	static const std::set<std::string> selfClosingElements;
	static const std::map<LexerTokenType, std::string> tokenTypeToString;

	/**
	 * Method that skips doctype declaration at the begining of document.
	 */
	void parseDoctypeDeclaration();

	/**
	 * Main method that handles parsing of elements, text, whitespaces.
	 * Parses from root of document excluding !DOCTYPE declaration.
	 * @returns position in the string where parsing was finished and from which may be continued outside this parser.
	 */
	unsigned int parseDocument();

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
	 * Method that parses word from current position given that the wors is quoted.
	 * @returns parsed word
	 */
	std::string parseQuotedWord();

	/**
	 * Method that skips all characters until it reaches > character. It will increment current position of parser
	 * until it reaches > character. Consider using it when IsNextCloseOpenedElement returns true or when you
	 * want to ignore attributes stored in current element.
	 */
	void CloseElement();

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

	void expectMoveToNextToken();
	bool expectTokensAvailable(unsigned count = 1);
	void expectTokenOfType(LexerTokenType type);

	void moveToNextToken();
	bool tokensAvailable(unsigned count = 1);
	LexerToken* currToken();
	LexerToken* nextToken();

	/**
	 * Registers parser error with logger used in application which then terminates execution of program.
	 * @parser message to be registered.
	 */
	void logError(std::string message);
};
#endif /* PARSER_PARSER_H_ */
