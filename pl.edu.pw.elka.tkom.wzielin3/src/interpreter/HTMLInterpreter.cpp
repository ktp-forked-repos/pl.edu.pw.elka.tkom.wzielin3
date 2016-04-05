/*
 * HTMLInterpreter.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "HTMLInterpreter.h"

HTMLInterpreter::HTMLInterpreter(HTMLElement* root)
{
	this->root = root;
}

HTMLInterpreter::~HTMLInterpreter()
{
}

const std::map<std::string, ThreatType> HTMLInterpreter::stringToThreatType =
{
{ "Botnet C&C", ThreatType::BotnetCC },
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
	if (tableRoot->innerElements.size() == 0
			|| tableRoot->innerElements[0]->name != "tbody")
	{
		throw "maintable doesn't containt tbody element";
	}
	HTMLElement* tbody = tableRoot->innerElements[0];
	std::vector<HTMLElement*> tableRows = tbody->innerElements;
	std::vector<ResultModel*> results;
	//skip first element because it is a header
	for (unsigned int i = 1; i < tableRows.size(); ++i)
	{
		ResultModel* model = interpretTableRow(tableRows[i]);
		results.push_back(model);
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
	model->host = tableRow->innerElements[3]->innerElements[1]->innerElements[0]->textContent;
	return model;
}

HTMLElement* HTMLInterpreter::getTableRoot(HTMLElement* startFrom)
{
	std::vector<HTMLAttribute*> attrs = startFrom->attributes;
	for (int i = 0; i < attrs.size(); ++i)
	{
		HTMLAttribute* attr = attrs[i];
		if(attr->name == "class")
		{
			std::vector<std::string> values = attr->values;
			for(int j = 0; j < values.size(); ++j)
			{
				std::string value = values[i];
				if(value == "maintable")
				{
					return startFrom;
				}
			}
		}
	}
	std::vector<HTMLElement*> innerElements = startFrom->innerElements;
	for(int i = 0; i < innerElements.size(); ++i)
	{
		HTMLElement* tableRoot = getTableRoot(innerElements[i]);
		if(tableRoot != nullptr)
		{
			return tableRoot;
		}
	}
	return nullptr;
}
