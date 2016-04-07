/*
 * HTMLInterpreter.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "HTMLInterpreter.h"
#include "../log/ConsoleLog.h"

HTMLInterpreter::HTMLInterpreter(HTMLElement* root)
{
	this->root = root;
}

HTMLInterpreter::~HTMLInterpreter()
{
}

const std::map<std::string, ThreatType> HTMLInterpreter::stringToThreatType =
{
{ "Botnet C&amp;C", ThreatType::BotnetCC },
{ "Payment Site", ThreatType::PaymentSite },
{ "Distribution Site", ThreatType::DistributionSite }, };
const std::map<std::string, MalwareType> HTMLInterpreter::stringToMalwareType =
{
{ "TeslaCrypt", MalwareType::TeslaCrypt },
{ "CryptoWall", MalwareType::CryptoWall },
{ "TorrentLocker", MalwareType::TorrentLocker },
{ "PadCrypt", MalwareType::PadCrypt },
{ "Locky", MalwareType::Locky },
{ "CTB-Locker", MalwareType::CTBLocker },
{ "FAKBEN", MalwareType::FAKBEN },
{ "PayCrypt", MalwareType::PayCrypt }, };

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
	std::string threatTypeString =
			tableRow->innerElements[1]->innerElements[0]->innerElements[0]->textContent;
	std::string malwareTypeString =
			tableRow->innerElements[2]->innerElements[0]->innerElements[0]->textContent;

	model->date = tableRow->innerElements[0]->innerElements[0]->textContent;
	model->threatType = stringToThreatType.find(threatTypeString)->second;
	model->malwareType = stringToMalwareType.find(malwareTypeString)->second;
	model->host =
			tableRow->innerElements[3]->innerElements[1]->innerElements[0]->textContent;
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
	std::string threatType = tableRow->innerElements[1]->innerElements[0]->innerElements[0]->textContent;
	if(stringToThreatType.find(threatType) == stringToThreatType.end())
	{
		return false;
	}
	//validate malwareType column
	if(tableRow->innerElements.size() < 3 || tableRow->innerElements[2]->innerElements.size() == 0
			|| tableRow->innerElements[2]->innerElements[0]->innerElements.size() == 0)
	{
		return false;
	}
	std::string malwareType = tableRow->innerElements[2]->innerElements[0]->innerElements[0]->textContent;
	if(stringToMalwareType.find(malwareType) == stringToMalwareType.end())
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
		if (attr->name == "class")
		{
			std::vector<std::string> values = attr->values;
			for (unsigned int j = 0; j < values.size(); ++j)
			{
				std::string value = values[j];
				if (value == "maintable")
				{
					if (startFrom->innerElements.size() > 0 && startFrom->innerElements[0]->name == "tbody")
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
