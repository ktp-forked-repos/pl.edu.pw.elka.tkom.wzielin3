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
#include"../lexer/Lexer.h"

class Parser
{
public:
	/**
	 * @param lexer to be used
	 * @param root HTMLElement to which parser will append parsed elements
	 */
	Parser(Lexer* lexer, HTMLElement* root);
	virtual ~Parser();

	/**
	 * Method that carries out parsing process.
	 */
	void parse();
private:
	HTMLElement* root;
	std::vector<LexerToken*> tokens;
	Lexer* lexer;
	unsigned int currPosition;

	static const std::set<std::string> selfClosingElements;
	static const std::map<LexerTokenType, std::string> tokenTypeToString;
	static const std::string DOCTYPE_TEXT;
	static const std::string SCRIPT_TEXT;

	/**
	 * Skips !DOCTYPE declaration if exists.
	 * Scans tags for next method to be called.
	 * Fetches new set of tokens for itself at the begining.
	 */
	void parseDoctypeDeclaration();

	/**
	 * Main method that handles parsing of elements, text, whitespaces.
	 * Parses from root of document excluding !DOCTYPE declaration.
	 * When called expects tokens in the tokens collection.
	 * Ends when no tokens available.
	 */
	void parseDocument();

	/**
	 * Method that extracts HTMLElement, that may have nested elements, from current position.
	 * Requires tokens in tokens collection. Requires first token to by [WORD] - current tag's name.
	 * Ends when reached end of tag. Doesn't scan tokens for next method.
	 * @param element HTMLElement where parsed data is saved
	 */
	void parseElement(HTMLElement* element);

	/**
	 * Method that extracts HTMLElement, that is a script element, from current position.
	 * Requires tokens in tokens collection. Requires first token to be [WORD] - text equal to 'script'.
	 * Ends when reach end of script - [</] tag. Doesn't scan tokens for next method.
	 * @param element HTMLElement where parsed data is saved
	 */
	void parseScript(HTMLElement* element);

	/**
	 * Method that extracts Attribute from current position for HTMLElement.
	 * Use when current position is inside HTMLElement and begins with attribute name.
	 * Requires tokens in tokens collection. Requires first token to be [WORD] - name of attribute.
	 * Ends when reached - ["] (unless it's escaped). Doesn't scan tokens for next method.
	 * @param attr HTMLAttrbiute to which parsed data is saved
	 * @param currentElementName name of an element that contains extracted attribute
	 */
	void parseAttribute(HTMLAttribute* attr, std::string currentElementName);

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
	 * If no more tokens than current are available reports an error,
	 * otherwise moves current pointer to next token.
	 */
	void expectMoveToNextToken();

	/**
	 * If token has a different type than specified reports an error.
	 */
	void expectTokenOfType(LexerTokenType type);

	/**
	 * Returns true if tokens are available
	 */
	bool tokensAvailable();

	/**
	 * Returns current token.
	 */
	LexerToken* currToken();

	/**
	 * Calls lexer to scan elements, assuming that current context is plain text.
	 * Sets current position pointer to 0 and sets new tokens collection.
	 */
	void lexerScanText();

	/**
	 * Calls lexer to scan elements, assuming that current context is inside a tag.
	 * Sets current position pointer to 0 and sets new tokens collection.
	 */
	void lexerScanTag();

	/**
	 * Calls lexer to scan elements, assuming that current context is a quote (either in tag or script).
	 * Sets current position pointer to 0 and sets new tokens collection.
	 */
	void lexerScanQuotation();

	/**
	 * Calls lexer to scan elements, assuming that current context is inside a script element.
	 * Sets current position pointer to 0 and sets new tokens collection.
	 */
	void lexerScanScript();

	/**
	 * Registers parser error with logger used in application which then terminates execution of program.
	 * @parser message to be registered.
	 */
	void logError(std::string message);

	/**
	 * Returns true if strings are equal case insensitive.
	 */
	bool equal(std::string a, std::string b);
};
#endif /* PARSER_PARSER_H_ */
