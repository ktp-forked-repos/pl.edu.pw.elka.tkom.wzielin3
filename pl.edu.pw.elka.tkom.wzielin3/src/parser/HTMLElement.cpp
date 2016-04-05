/*
 * HTMLElement.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "HTMLElement.h"

HTMLElement::HTMLElement()
{
	// TODO Auto-generated constructor stub

}

HTMLElement::~HTMLElement()
{
	while(attributes.size() > 0)
	{
		HTMLAttribute* attr = attributes[attributes.size() - 1];
		attributes.pop_back();
		delete attr;
	}
	while(innerElements.size() > 0)
	{
		HTMLElement* element = innerElements[innerElements.size() - 1];
		innerElements.pop_back();
		delete element;
	}
}

