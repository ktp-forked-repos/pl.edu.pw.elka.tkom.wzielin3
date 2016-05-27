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

void ConsoleLog::logError(std::string message)
{
	std::cerr << message << "\n";
	exit(1);
}
