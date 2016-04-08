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
	 * Default constructor
	 * @params toParse html to scan
	 */
	Lexer(std::string toParse);
	virtual ~Lexer();

	/**
	 * Method used for retrieving lexical tokens from html input.
	 * @returns collection of ordered lexical tokens
	 */
	std::vector<LexerToken*> scan();

private:
	static const char OPEN_TAG = '<';
	static const char CLOSE_TAG = '>';
	static const char FORWARD_SLASH = '/';
	static const char QUOTATION_MARK = '"';
	static const char EQUAL_SIGN = '=';

	unsigned int currPosition;
	std::string toParse;
	std::vector<LexerToken*> tokens;

	void scanForWord();
	void scanForOpenTag();
	void scanForCloseTag();
	void scanForOpenSlashedTag();
	void scanForClosedSlashedTag();
	void scanForQuoteSign();
	void scanForEqualSign();
	void scanForWhitespace();

	bool isNextWord();
	bool isNextOpenTag();
	bool isNextCloseTag();
	bool isNextOpenSlashedTag();
	bool isNextClosedSlashedTag();
	bool isNextQuoteSign();
	bool isNextEqualSign();
	bool isNextWhitespace();

	/**
	 * Registers lexer error with logger used in application which then terminates execution of program.
	 * @parser message to be registered.
	 */
	void logError(std::string message);
};

#endif /* LEXER_LEXER_H_ */

