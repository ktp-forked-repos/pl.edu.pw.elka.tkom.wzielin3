/*
 * ConfigurationManager.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: wojciech
 */

#include<algorithm>
#include "ConfigurationManager.h"
#include "../log/ConsoleLog.h"

const std::map<std::string, ThreatType> ConfigurationManager::stringToThreatType =
{
{ "botnetcc", ThreatType::BotnetCC },
{ "paymentsite", ThreatType::PaymentSite },
{ "distributionsite", ThreatType::DistributionSite }, };
const std::map<std::string, MalwareType> ConfigurationManager::stringToMalwareType =
{
{ "teslacrypt", MalwareType::TeslaCrypt },
{ "cryptowall", MalwareType::CryptoWall },
{ "torrentlocker", MalwareType::TorrentLocker },
{ "padcrypt", MalwareType::PadCrypt },
{ "locky", MalwareType::Locky },
{ "ctblocker", MalwareType::CTBLocker },
{ "fakben", MalwareType::FAKBEN },
{ "paycrypt", MalwareType::PayCrypt }, };

ConfigurationManager::ConfigurationManager(int argc, char** argv)
{
	webSiteUrl = "ransomwaretracker.abuse.ch/tracker";
	applyMalwareFilter = false;
	applyThreatFilter = false;

	for(int i = 1; i < argc - 1; i += 2)
	{
		std::string command = GetLowerCase(argv[i]);
		std::string value = GetLowerCase(argv[i + 1]);
		if(command == "-malware")
		{
			ConfigureMalwareFilter(value);
		}
		else if(command == "-threat")
		{
			ConfigureThreatFilter(value);
		}
		else if(command == "-url")
		{
			webSiteUrl = value;
		}
		else
		{
			ConsoleLog log;
			log.logError("Command " + command + " not found. Available commands: -malware, -threat, -url.");
		}
	}
}

ConfigurationManager::~ConfigurationManager()
{
	// TODO Auto-generated destructor stub
}

void ConfigurationManager::ConfigureMalwareFilter(std::string argv)
{
	std::map<std::string, MalwareType>::const_iterator it = stringToMalwareType.find(argv);
	if(it != stringToMalwareType.end())
	{
		applyMalwareFilter = true;
		malwareType = it->second;
	}
	else
	{
		ConsoleLog log;
		log.logError("Malware filter " + argv +  " not found. Must be one of following: TeslaCrypt, "
				"CryptoWall, TorrentLocker, PadCrypt, Locky, CTBLocker, FAKBEN, PayCrypt.");
	}
}

void ConfigurationManager::ConfigureThreatFilter(std::string argv)
{
	std::map<std::string, ThreatType>::const_iterator it = stringToThreatType.find(argv);
	if(it != stringToThreatType.end())
	{
		applyThreatFilter = true;
		threaType = it->second;
	}
	else
	{
		ConsoleLog log;
		log.logError("Threat filter " + argv +  " not found. Must be one of following: "
				"BotnetCC, PaymentSite, DistributionSite.");
	}
}

std::string ConfigurationManager::GetLowerCase(char* arg)
{
	std::string str(arg);
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}
