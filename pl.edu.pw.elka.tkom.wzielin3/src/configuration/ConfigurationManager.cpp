/*
 * ConfigurationManager.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: wojciech
 */

#include "ConfigurationManager.h"

const std::map<std::string, ThreatType> ConfigurationManager::stringToThreatType =
{
{ "BotnetCC", ThreatType::BotnetCC },
{ "PaymentSite", ThreatType::PaymentSite },
{ "DistributionSite", ThreatType::DistributionSite }, };
const std::map<std::string, MalwareType> ConfigurationManager::stringToMalwareType =
{
{ "TeslaCrypt", MalwareType::TeslaCrypt },
{ "CryptoWall", MalwareType::CryptoWall },
{ "TorrentLocker", MalwareType::TorrentLocker },
{ "PadCrypt", MalwareType::PadCrypt },
{ "Locky", MalwareType::Locky },
{ "CTBLocker", MalwareType::CTBLocker },
{ "FAKBEN", MalwareType::FAKBEN },
{ "PayCrypt", MalwareType::PayCrypt }, };

ConfigurationManager::ConfigurationManager(int argc, char** argv)
{
	webSiteUrl = "src/HtmlContent.html";//"ransomwaretracker.abuse.ch/tracker";
	applyMalwareFilter = false;
	applyThreatFilter = false;

	for(unsigned int i = 1; i < argc; ++i)
	{
		if(strcmp(argv[i], "-MalwareFilter") == 0)
		{
			i++;
			ConfigureMalwareFilter(argv[i]);
		}
		else if(strcmp(argv[i], "-ThreatFilter") == 0)
		{
			i++;
			ConfigureThreatFilter(argv[i]);
		}
		else if(strcmp(argv[i], "-WebSiteUrl") == 0)
		{
			i++;
			webSiteUrl = argv[i];
		}
	}
}

ConfigurationManager::~ConfigurationManager()
{
	// TODO Auto-generated destructor stub
}

void ConfigurationManager::ConfigureMalwareFilter(char* argv)
{
	std::map<std::string, MalwareType>::const_iterator it = stringToMalwareType.find(argv);
	if(it != stringToMalwareType.end())
	{
		applyMalwareFilter = true;
		malwareType = it->second;
	}
}

void ConfigurationManager::ConfigureThreatFilter(char* argv)
{
	std::map<std::string, ThreatType>::const_iterator it = stringToThreatType.find(argv);
	if(it != stringToThreatType.end())
	{
		applyThreatFilter = true;
		threaType = it->second;
	}
}
