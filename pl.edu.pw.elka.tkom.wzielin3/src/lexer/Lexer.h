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

	std::vector<LexerToken*> scanText();
	std::vector<LexerToken*> scanTag();
	std::vector<LexerToken*> scanQuotation();
	std::vector<LexerToken*> scanScript();
	bool isFinished();

private:
	static const char OPEN_TAG = '<';
	static const char CLOSE_TAG = '>';
	static const char FORWARD_SLASH = '/';
	static const char ESCAPE_SIGN = '\\';
	static const char QUOTATION_MARK = '"';
	static const char EQUAL_SIGN = '=';

	unsigned int currPosition;
	std::string toParse;
	std::vector<LexerToken*> tokens;

	void saveWordFrom(unsigned int startPosition);

	bool scanForWordQuoted();
	bool scanForWord();
	bool scanForOpenTag();
	bool scanForCloseTag();
	bool scanForOpenSlashedTag();
	bool scanForClosedSlashedTag();
	bool scanForQuoteSign();
	bool scanForEqualSign();
	bool scanForWhitespace();

	bool isNextWordQuoted();
	bool isNextWord();
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
	void deleteCurrentTokens();
};

#endif /* LEXER_LEXER_H_ */

