/*
 * DetailsInterpreter.h
 *
 *  Created on: May 27, 2016
 *      Author: wojciech
 */

#ifndef INTERPRETER_DETAILS_DETAILSINTERPRETER_H_
#define INTERPRETER_DETAILS_DETAILSINTERPRETER_H_

#include "DetailsResultModel.h"
#include "../parser/HTMLElement.h"
/**
 * Class used for interpreting data received from 'details' page
 * of ransomwaretracker.abuse.ch. for example http://ransomwaretracker.abuse.ch/host/12hourenergy.com.au/
 */
class DetailsInterpreter
{
public:
	DetailsInterpreter(HTMLElement* root);
	virtual ~DetailsInterpreter();

	/**
	 * Interprets HTMLElement specified in constructor and generates model for the page
	 */
	DetailsResultModel interpret();
private:
	HTMLElement *rootElement;

	HTMLElement* getTableRoot(HTMLElement* startFrom);
};

#endif /* INTERPRETER_DETAILS_DETAILSINTERPRETER_H_ */
