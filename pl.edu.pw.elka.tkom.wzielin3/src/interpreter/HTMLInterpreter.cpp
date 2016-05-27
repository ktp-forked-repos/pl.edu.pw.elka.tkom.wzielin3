/*
 * HTMLInterpreter.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "HTMLInterpreter.h"
#include "../log/ConsoleLog.h"
#include "../common/Utils.h"

HTMLInterpreter::HTMLInterpreter(HTMLElement* root)
{
	this->root = root;
}

HTMLInterpreter::~HTMLInterpreter()
{
}

std::vector<ResultModel*> HTMLInterpreter::interpret()
{
	HTMLElement* tableRoot = getTableRoot(root);
	if (tableRoot == nullptr)
	{
		ConsoleLog log;
		log.logError("Table root not found in parsed HTML. HTML must have an element with a class equal to maintable.");
	}
	std::vector<HTMLElement*> tableRows = tableRoot->innerElements;
	std::vector<ResultModel*> results;
	//skip first element because it is a header
	for (unsigned int i = 1; i < tableRows.size(); ++i)
	{
		if(isValidTableRow(tableRows[i]))
		{
			ResultModel* model = interpretTableRow(tableRows[i]);
			results.push_back(model);
		}
	}
	return results;
}

ResultModel* HTMLInterpreter::interpretTableRow(HTMLElement* tableRow)
{
	ResultModel* model = new ResultModel();

	model->date = tableRow->innerElements[0]->innerElements[0]->textContent;
	model->threatType = tableRow->innerElements[1]->innerElements[0]->innerElements[0]->textContent;
	model->malwareType = tableRow->innerElements[2]->innerElements[0]->innerElements[0]->textContent;
	model->host = tableRow->innerElements[3]->innerElements[1]->innerElements[0]->textContent;
	return model;
}

bool HTMLInterpreter::isValidTableRow(HTMLElement* tableRow)
{
	//validate date column
	if(tableRow->innerElements.size() < 1 || tableRow->innerElements[0]->innerElements.size() == 0)
	{
		return false;
	}
	//validate threatType column
	if(tableRow->innerElements.size() < 2 || tableRow->innerElements[1]->innerElements.size() == 0
			|| tableRow->innerElements[1]->innerElements[0]->innerElements.size() == 0)
	{
		return false;
	}
	//validate malwareType column
	if(tableRow->innerElements.size() < 3 || tableRow->innerElements[2]->innerElements.size() == 0
			|| tableRow->innerElements[2]->innerElements[0]->innerElements.size() == 0)
	{
		return false;
	}
	//validate host colum
	if(tableRow->innerElements.size() < 4 || tableRow->innerElements[3]->innerElements.size() < 2 ||
			tableRow->innerElements[3]->innerElements[1]->innerElements.size() < 1)
	{
		return false;
	}
	return true;
}

HTMLElement* HTMLInterpreter::getTableRoot(HTMLElement* startFrom)
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
				if (Utils::stringsEqual(value, "maintable"))
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
