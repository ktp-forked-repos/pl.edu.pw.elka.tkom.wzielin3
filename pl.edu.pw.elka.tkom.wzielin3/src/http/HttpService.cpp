/*
 * HttpService.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: wojciech
 */

#include<stdio.h>
#include<stdlib.h>
#include "HttpService.h"

HttpService::HttpService()
{
	// TODO Auto-generated constructor stub

}

HttpService::~HttpService()
{
	// TODO Auto-generated destructor stub
}

std::string HttpService::getHtml(std::string url)
{
	FILE *fp;
	char buffer[1024];
	std::string result = "";
	std::string command = "curl -X GET " + url + " 2> /dev/null";

	/* Open the command for reading. */
	fp = popen(command.c_str(), "r");
	if (fp == NULL)
	{
		std::cout << "Failed to run curl command.";
		exit(1);
	}

	/* Read the output a line at a time - output it. */
	while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL)
	{
		result += buffer;
	}
	pclose(fp);

	return result;
}
