/*
 * ConfigurationManager.h
 *
 *  Created on: Apr 4, 2016
 *      Author: wojciech
 */

#ifndef CONFIGURATIONMANAGER_H_
#define CONFIGURATIONMANAGER_H_

#include<iostream>
#include"interpreter/ResultModel.h"

/**
 * Class representing configuration of currently run program.
 */
class ConfigurationManager
{
public:
	/**
	 * Default constructor.
	 * @param argc command line arguments count
	 * @param argv command line arguments
	 */
	ConfigurationManager(int argc, char** argv);
	virtual ~ConfigurationManager();

	bool applyMalwareFilter;
	bool applyThreatFilter;
	MalwareType malwareType;
	ThreatType threaType;
	std::string webSiteUrl;
private:

	void ConfigureMalwareFilter(char* argv);
	void ConfigureThreatFilter(char* argv);

};

#endif /* CONFIGURATIONMANAGER_H_ */
