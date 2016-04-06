/*
 * ConsoleUI.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "ConsoleLog.h"

#include<iostream>

ConsoleLog::ConsoleLog()
{
	// TODO Auto-generated constructor stub

}

ConsoleLog::~ConsoleLog()
{
	// TODO Auto-generated destructor stub
}

const std::map<ThreatType, std::string> ConsoleLog::threatTypeToString =
{
{ ThreatType::BotnetCC, "BotnetCC" },
{ ThreatType::PaymentSite, "PaymentSite" },
{ ThreatType::DistributionSite, "DistributionSite" }, };
const std::map<MalwareType, std::string> ConsoleLog::malwareTypeToString =
{
{ MalwareType::TeslaCrypt, "TeslaCrypt" },
{ MalwareType::CryptoWall, "CryptoWall" },
{ MalwareType::TorrentLocker, "TorrentLocker" },
{ MalwareType::PadCrypt, "PadCrypt" },
{ MalwareType::Locky, "Locky" },
{ MalwareType::CTBLocker, "CTBLocker" },
{ MalwareType::FAKBEN, "FAKBEN" },
{ MalwareType::PayCrypt, "PayCrypt" }, };

void ConsoleLog::logResults(std::vector<ResultModel*> models)
{
	std::cout << "{\"results\":[";
	if (models.size() > 0)
	{
		logResult(models[0]);
	}
	for (int i = 1; i < models.size(); ++i)
	{
		std::cout << ",";
		logResult(models[i]);
	}
	std::cout << "]}";
}

void ConsoleLog::logResult(ResultModel* model)
{
	std::cout << "{\"date\":\"" << model->date
			<< "\",\"threat\":\"" << threatTypeToString.find(model->threatType)->second
			<< "\",\"malware\":\"" << malwareTypeToString.find(model->malwareType)->second
			<< "\",\"host\":\"" << model->host
			<< "\"}";
}

void ConsoleLog::logError(std::string message)
{
	std::cerr << message;
	exit(1);
}
