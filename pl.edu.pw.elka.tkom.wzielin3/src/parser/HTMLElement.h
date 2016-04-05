/*
 * HTMLElement.h
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#ifndef PARSER_HTMLELEMENT_H_
#define PARSER_HTMLELEMENT_H_
#include"HTMLAttribute.h"

class HTMLElement
{
public:
	HTMLElement();
	virtual ~HTMLElement();

	std::string name;
	std::vector<HTMLElement*> innerElements;
	std::vector<HTMLAttribute*> attributes;
	std::string textContent;
};

#endif /* PARSER_HTMLELEMENT_H_ */
