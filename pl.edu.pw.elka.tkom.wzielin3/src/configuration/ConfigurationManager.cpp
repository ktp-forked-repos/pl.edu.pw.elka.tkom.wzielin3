/*
 * ConfigurationManager.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: wojciech
 */

#include<algorithm>
#include "ConfigurationManager.h"
#include "../log/ConsoleLog.h"
#include "../common/Utils.h"

ConfigurationManager::ConfigurationManager(int argc, char** argv)
{
	path = "ransomwaretracker.abuse.ch/tracker";
	applyMalwareFilter = false;
	applyThreatFilter = false;

	for (int i = 1; i < argc - 1; i += 2)
	{
		std::string command = argv[i];
		std::string value = argv[i + 1];
		if (Utils::stringsEqual(command, "-malware"))
		{
			ConfigureMalwareFilter(value);
		}
		else if (Utils::stringsEqual(command, "-threat"))
		{
			ConfigureThreatFilter(value);
		}
		else if (Utils::stringsEqual(command, "-url"))
		{
			path = value;
		}
		else if(Utils::stringsEqual(command, "-file"))
		{
			path = value;
			readFromFile = true;
		}
		else
		{
			ConsoleLog log;
			log.logError(
					"Command " + command
							+ " not found. Available commands: -malware, -threat, -url.");
		}
	}
}

ConfigurationManager::~ConfigurationManager()
{
}

void ConfigurationManager::ConfigureMalwareFilter(std::string argv)
{
	if (argv.size() > 0)
	{
		applyMalwareFilter = true;
		malwareType = argv;
	}
}

void ConfigurationManager::ConfigureThreatFilter(std::string argv)
{
	if (argv.size() > 0)
	{
		applyThreatFilter = true;
		threaType = argv;
	}
}
