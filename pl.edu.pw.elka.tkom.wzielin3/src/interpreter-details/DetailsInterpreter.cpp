/*
 * DetailsInterpreter.cpp
 *
 *  Created on: May 27, 2016
 *      Author: wojciech
 */

#include "DetailsInterpreter.h"
#include "../log/ConsoleLog.h"
#include "../common/Utils.h"

DetailsInterpreter::DetailsInterpreter(HTMLElement *rootElement)
{
	this->rootElement = rootElement;
}

DetailsInterpreter::~DetailsInterpreter()
{
	// TODO Auto-generated destructor stub
}


DetailsResultModel DetailsInterpreter::interpret()
{
	HTMLElement* tableRoot = getTableRoot(rootElement);
	if (tableRoot == nullptr)
	{
		ConsoleLog().logError(
				"Table root not found in parsed HTML. HTML must have an element with a class equal to tablehost.");
	}
	if(tableRoot->innerElements.size() < 4 || tableRoot->innerElements[3]->innerElements.size() < 2
			|| tableRoot->innerElements[3]->innerElements[1]->innerElements.empty())
	{
		ConsoleLog().logError(
				"Details displayed in table don't contain URL info. Text must be specified in a fourth row, second column");

	}
	std::string url = tableRoot->innerElements[3]->innerElements[1]->innerElements[0]->textContent;
	DetailsResultModel model;
	model.url = url;
	return model;
}


HTMLElement* DetailsInterpreter::getTableRoot(HTMLElement* startFrom)
{
	std::vector<HTMLAttribute*> attrs = startFrom->attributes;
	for (unsigned int i = 0; i < attrs.size(); ++i)
	{
		HTMLAttribute* attr = attrs[i];
		if (Utils::stringsEqual(attr->name, "class"))
		{
			std::vector<std::string> values = attr->values;
			for (unsigned int j = 0; j < values.size(); ++j)
			{
				std::string value = values[j];
				if (Utils::stringsEqual(value, "tablehost"))
				{
					if (startFrom->innerElements.size() > 0
							&& Utils::stringsEqual(startFrom->innerElements[0]->name, "tbody"))
					{
						return startFrom->innerElements[0];
					}
					return startFrom;
				}
			}
		}
	}
	std::vector<HTMLElement*> innerElements = startFrom->innerElements;
	for (unsigned int i = 0; i < innerElements.size(); ++i)
	{
		HTMLElement* tableRoot = getTableRoot(innerElements[i]);
		if (tableRoot != nullptr)
		{
			return tableRoot;
		}
	}
	return nullptr;
}
