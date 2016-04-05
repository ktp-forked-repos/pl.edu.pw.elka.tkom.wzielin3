/*
 * ConfigurationManager.h
 *
 *  Created on: Apr 4, 2016
 *      Author: wojciech
 */

#ifndef CONFIGURATIONMANAGER_H_
#define CONFIGURATIONMANAGER_H_

#include<iostream>
#include<map>
#include<string.h>
#include"../interpreter/ResultModel.h"

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

	/**
	 * Sets filtering by malware type configuration.
	 * @param malware filter argv argument received in command line
	 */
	void ConfigureMalwareFilter(char* argv);

	/**
	 * Sets filtering by threat type configuration.
	 * @param threat filter argv argument received in command line
	 */
	void ConfigureThreatFilter(char* argv);

	static const std::map<std::string, ThreatType> stringToThreatType;
	static const std::map<std::string, MalwareType> stringToMalwareType;

};

#endif /* CONFIGURATIONMANAGER_H_ */
