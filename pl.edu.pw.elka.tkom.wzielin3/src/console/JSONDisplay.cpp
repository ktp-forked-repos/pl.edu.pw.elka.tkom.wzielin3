/*
 * ConsoleUI.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "JSONDisplay.h"

#include<iostream>

JSONDisplay::JSONDisplay()
{
	// TODO Auto-generated constructor stub

}

JSONDisplay::~JSONDisplay()
{
	// TODO Auto-generated destructor stub
}

const std::map<ThreatType, std::string> JSONDisplay::threatTypeToString =
{
{ ThreatType::BotnetCC, "BotnetCC" },
{ ThreatType::PaymentSite, "PaymentSite" },
{ ThreatType::DistributionSite, "DistributionSite" }, };
const std::map<MalwareType, std::string> JSONDisplay::malwareTypeToString =
{
{ MalwareType::TeslaCrypt, "TeslaCrypt" },
{ MalwareType::CryptoWall, "CryptoWall" },
{ MalwareType::TorrentLocker, "TorrentLocker" },
{ MalwareType::PadCrypt, "PadCrypt" },
{ MalwareType::Locky, "Locky" },
{ MalwareType::CTBLocker, "CTBLocker" },
{ MalwareType::FAKBEN, "FAKBEN" },
{ MalwareType::PayCrypt, "PayCrypt" }, };

void JSONDisplay::display(std::vector<ResultModel*> models)
{
	std::cout << "{\"results\":[";
	if (models.size() > 0)
	{
		display(models[0]);
	}
	for (int i = 1; i < models.size(); ++i)
	{
		std::cout << ",";
		display(models[i]);
	}
	std::cout << "]}";
}

void JSONDisplay::display(ResultModel* model)
{
	std::cout << "{\"date\":\"" << model->date
			<< "\",\"threat\":\"" << threatTypeToString.find(model->threatType)->second
			<< "\",\"malware\":\"" << malwareTypeToString.find(model->malwareType)->second
			<< "\",\"host\":\"" << model->host
			<< "\"}";
}
