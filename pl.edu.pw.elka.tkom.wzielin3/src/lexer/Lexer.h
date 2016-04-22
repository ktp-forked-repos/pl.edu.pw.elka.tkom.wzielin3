/*
 * HTMLLexer.h
 *
 *  Created on: Apr 7, 2016
 *      Author: wojciech
 */

#ifndef LEXER_LEXER_H_
#define LEXER_LEXER_H_

#include<vector>
#include<set>
#include"LexerToken.h"

class Lexer
{
public:
	/**
	 * @params toParse html to scan
	 */
	Lexer(std::string toParse);
	virtual ~Lexer();

	std::vector<LexerToken*> scanText();
	std::vector<LexerToken*> scanTag();
	std::vector<LexerToken*> scanHTMLQuote();
	std::vector<LexerToken*> scanScript();
	void skipDoctype();

private:
	static const char OPEN_TAG = '<';
	static const char CLOSE_TAG = '>';
	static const char FORWARD_SLASH = '/';
	static const char ESCAPE_SIGN = '\\';
	static const char QUOTATION_MARK = '"';
	static const char EQUAL_SIGN = '=';
	static const std::string OPEN_COMMENT;
	static const std::string CLOSE_COMMENT;
	static const std::string DOCTYPE_OPEN;

	unsigned int currPosition;
	std::string toParse;

	/**
	 * tokens generated in current scope
	 */
	std::vector<LexerToken*> tokens;

	/**
	 * saves a token WORD which starts at startPosition and finishes at currentPosition (exclusive)
	 */
	void saveWordFrom(unsigned int startPosition);
	/**
	* Internal use for scanning quotes in JavaScript. Doesn't add new tokens just increments position.
	*/
	void scanJSQuote();

	/**
	 * Methods that check if next characters are specified tokens and if so they scan them and
	 * create tokens.
	 */
	bool scanForOpenTag();
	bool scanForCloseTag();
	bool scanForOpenSlashedTag();
	bool scanForClosedSlashedTag();
	bool scanForQuoteSign();
	bool scanForEqualSign();

	/**
	 * Methods that increment current position and ignore following elements.
	 */
	bool skipWhitespaces();
	bool skipSingleComment();
	void skipWhitespacesAndComments();

	/**
	 * method that check if next characters are specified tokens.
	 */
	bool isNextOpenTag();
	bool isNextCloseTag();
	bool isNextOpenSlashedTag();
	bool isNextClosedSlashedTag();
	bool isNextQuoteSign();
	bool isNextEqualSign();
	bool isNextWhitespace();
	bool isNextEscapeSign();

	/**
	 * Registers lexer error with logger used in application which then terminates execution of program.
	 * @parser message to be registered.
	 */
	void logError(std::string message);

	/**
	 * Deletes contents of tokens vector - used when next scope is to be scanned and previous tokens
	 * are no longer needed.
	 */
	void deleteCurrentTokens();
};

#endif /* LEXER_LEXER_H_ */

