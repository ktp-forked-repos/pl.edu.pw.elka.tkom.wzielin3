/*
 * HTMLInterpreter.h
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#ifndef INTERPRETER_HTMLINTERPRETER_H_
#define INTERPRETER_HTMLINTERPRETER_H_

#include<vector>
#include<map>
#include "ResultModel.h"
#include "../parser/HTMLElement.h"

/**
 * Class used for interpreting data received from home page (where a list of threats is displaed)
 * of ransomwaretracker.abuse.ch. for example http://ransomwaretracker.abuse.ch/tracker/page/1/
 */
class HTMLInterpreter
{
public:
	/**
	 * Default Constructor for HTMLInterpreter.
	 * @param root root HTMLElement for document to be interpreted
	 */
	HTMLInterpreter(HTMLElement* root);
	virtual ~HTMLInterpreter();

	/**
	 * Interprets HTMLElement specified in constructor and generates models
	 * of entries of the table in parsed html document.
	 */
	std::vector<ResultModel*> interpret();

private:
	//Root of the whole document
	HTMLElement* root;

	/**
	 * Method that looks for a table with a class of "maintable"
	 * @param startFrom HTMLElement that should be searched
	 * @returns main table from HTML document, or nullptr if not found
	 */
	HTMLElement* getTableRoot(HTMLElement* startFrom);

	/**
	 * Method that interprets single row from HTML table.
	 * @param tableRow HTMLElement from which to generated data
	 * @returns generatet ResultModel data from tableRow
	 */
	ResultModel* interpretTableRow(HTMLElement* tableRow);

	/**
	 * Method that checks if a HTMLElement is a properly structured table row.
	 * @param tableRow HTMLElement to validate
	 * @returns true if the HTMLElement is a valid tableRow, false otherwise.
	 */
	bool isValidTableRow(HTMLElement* tableRow);


};

#endif /* INTERPRETER_HTMLINTERPRETER_H_ */
