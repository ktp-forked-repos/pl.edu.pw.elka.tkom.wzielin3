/*
 * HttpService.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: wojciech
 */

#include<stdio.h>
#include<stdlib.h>
#include "HttpService.h"
#include"../log/ConsoleLog.h"

HttpService::HttpService()
{
}

HttpService::~HttpService()
{
}

std::string HttpService::getHtml(std::string url)
{
	FILE *fp;
	char buffer[1024];
	std::string result = "";
	std::string command = "curl " + url + " 2> /dev/null";

	/* Open the command for reading. */
	fp = popen(command.c_str(), "r");
	if (fp == NULL)
	{
		ConsoleLog log;
		log.logError("Failed open curl command for reading.");
	}

	/* Read the output a line at a time - output it. */
	while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL)
	{
		result += buffer;
	}
	pclose(fp);

	if(result.size() == 0)
	{
		ConsoleLog log;
		log.logError("No result received from curl command. Make sure you have internet access, have properly installed curl "
				"and specified a proper web page to be parsed. Default page is http://ransomwaretracker.abuse.ch/tracker/.");
	}

	return result;
}
