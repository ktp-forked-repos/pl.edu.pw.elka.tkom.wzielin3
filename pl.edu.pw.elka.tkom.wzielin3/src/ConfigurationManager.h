/*
 * ConfigurationManager.h
 *
 *  Created on: Apr 4, 2016
 *      Author: wojciech
 */

#ifndef CONFIGURATIONMANAGER_H_
#define CONFIGURATIONMANAGER_H_

#include<iostream>

class ConfigurationManager
{
public:
	ConfigurationManager();
	virtual ~ConfigurationManager();

	std::string getWebsiteUrl();
	std::string getLogFilePath();
};

#endif /* CONFIGURATIONMANAGER_H_ */
