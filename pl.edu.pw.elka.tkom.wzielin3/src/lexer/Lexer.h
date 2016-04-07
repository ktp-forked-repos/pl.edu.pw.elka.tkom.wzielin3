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
	static const char BACKWARD_SLASH = '\\';
	static const char QUOTATION_MARK = '"';
	static const char EQUAL_SIGN = '=';

	static const std::set<char> specialCharacters;

	unsigned int currPosition;
	std::string toParse;
	std::vector<LexerToken*> tokens;

	/**
	 * Method that adds OPEN_TAG token to collection of tokens if found one and increments current position.
	 * @return true if found token.
	 */
	bool scanForOpenTag();
	bool scanForCloseTag();
	bool scanForOpenSlashedTag();
	bool scanForClosedSlashedTag();
	bool scanForWord();
	bool scanForQuoteSign();
	bool scanForQuotedWord();
	bool scanForEqualSign();
	bool scanForWhitespace();
	bool scanForBackwardSlash();
	bool scanForForwardSlash();
};

#endif /* LEXER_LEXER_H_ */

