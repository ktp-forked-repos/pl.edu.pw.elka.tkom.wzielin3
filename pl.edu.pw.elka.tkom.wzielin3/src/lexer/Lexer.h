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
#include<list>
#include"LexerToken.h"

class Lexer
{
public:
	/**
	 * @params toParse html to scan
	 */
	Lexer(std::string toParse);
	virtual ~Lexer();

	LexerToken scanText();
	LexerToken scanTag();
	LexerToken scanHTMLQuote();
	LexerToken scanScript();
	void skipDoctype();
	bool eof();
	unsigned getCurrentPosition();

private:
	static const std::string OPEN_TAG;
	static const std::string OPEN_SLASHED_TAG;
	static const std::string CLOSE_TAG;
	static const std::string CLOSE_SLASHED_TAG;
	static const std::string FORWARD_SLASH;
	static const std::string ESCAPE_SIGN;
	static const std::string QUOTE_SIGN;
	static const std::string EQUAL_SIGN;
	static const std::string OPEN_COMMENT;
	static const std::string CLOSE_COMMENT;
	static const std::string DOCTYPE_OPEN;

	static const std::list<std::pair<LexerTokenType, std::string> > textScopeTokens;
	static const std::list<std::pair<LexerTokenType, std::string> > tagScopeTokens;
	static const std::list<std::pair<LexerTokenType, std::string> >htmlQuoteScopeTokens;
	static const std::list<std::pair<LexerTokenType, std::string> >scriptScopeTokens;

	unsigned int currPosition;
	std::string toParse;

	LexerToken scanScope(std::list<std::pair<LexerTokenType, std::string> > tokens, bool endOnWhitespace);
	LexerToken scanFor(std::list<std::pair<LexerTokenType, std::string> > tokens);
	bool scanFor(std::string word);
	bool isNext(std::list<std::pair<LexerTokenType, std::string> > tokens);
	bool isNext(std::string word);
	bool isNextWhitespace();

	/**
	 * returns a token WORD which starts at startPosition and finishes at currentPosition (exclusive)
	 */
	LexerToken getWordFrom(unsigned int startPosition);

	/**
	 * Methods that increment current position if specified elements are present.
	 */
	void skipScriptQuote();
	bool skipWhitespaces();
	bool skipComment();
	void skipWhitespacesAndComments();
};

#endif /* LEXER_LEXER_H_ */

