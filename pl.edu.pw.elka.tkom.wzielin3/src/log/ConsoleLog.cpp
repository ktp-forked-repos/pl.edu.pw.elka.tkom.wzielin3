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
}

ConsoleLog::~ConsoleLog()
{
}

void ConsoleLog::logResults(std::vector<ResultModel*> models)
{
	std::cout << "{\"results\":[";
	if (models.size() > 0)
	{
		logResult(models[0]);
	}
	for (unsigned int i = 1; i < models.size(); ++i)
	{
		std::cout << ",";
		logResult(models[i]);
	}
	std::cout << "]}";
}

void ConsoleLog::logResult(ResultModel* model)
{
	std::cout << "{\"date\":\"" << model->date
			<< "\",\"threat\":\"" << model->threatType
			<< "\",\"malware\":\"" << model->malwareType
			<< "\",\"host\":\"" << model->host
			<< "\"}";
}

void ConsoleLog::logResult(DetailsResultModel model)
{
	std::cout << "{\"url\":\"" << model.url << "\"}";
}

void ConsoleLog::logError(std::string message)
{
	std::cerr << message << "\n";
	exit(1);
}

void ConsoleLog::logHelp()
{
	std::cerr << "pl.edu.pw.elka.tkom.wzielin3 [-threat ThreatString] [-malware MalwareString] [-file FilePath] [-url Url] [-single]\n";
	std::cerr << "-threat - optionally specifies threat by which results should be filtered. applicable only if -single is not specified.\n";
	std::cerr << "-malware - optionally specifies malware by which results should be filtered. applicable only if -single is not specified.\n";
	std::cerr << "-file - optionally specifies file path from which input document should be read. by default document is downloaded from web.\n";
	std::cerr << "-url - optionally specifies url from which input document should be read. applicable only if -file is not specified.";
	std::cerr << " by default home page is downloaded\n";
	std::cerr << "-single - should be specified if document should be interpreted as details page.";
	std::cerr << " otherwise document is interpreted as home page document - list of all malwares.\n";
	exit(0);
}
