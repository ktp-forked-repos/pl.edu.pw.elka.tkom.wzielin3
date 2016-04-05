/*
 * ConfigurationManager.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: wojciech
 */

#include "ConfigurationManager.h"

ConfigurationManager::ConfigurationManager(int argc, char** argv)
{
	webSiteUrl = "ransomwaretracker.abuse.ch/tracker";
	applyMalwareFilter = false;
	applyThreatFilter = false;

	for(unsigned int i = 0; i < argc; ++i)
	{
		if(argv[i] == "-MalwareFilter")
		{
			i++;
			ConfigureMalwareFilter(argv[i]);
		}
		else if(argv[i] == "-ThreatFilter")
		{
			i++;
			ConfigureThreatFilter(argv[i]);
		}
	}
}

ConfigurationManager::~ConfigurationManager()
{
	// TODO Auto-generated destructor stub
}

void ConfigurationManager::ConfigureMalwareFilter(char* argv)
{
	//TODO
}
void ConfigurationManager::ConfigureThreatFilter(char* argv)
{
	//todo
}
