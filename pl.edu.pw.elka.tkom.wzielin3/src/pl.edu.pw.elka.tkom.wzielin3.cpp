//============================================================================
// Name        : wzielin3.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ConfigurationManager.h"
#include "HttpService.h"
#include "parser/HTMLParser.h"

int main() {
	ConfigurationManager configuration;
	HttpService http;
	HTMLParser parser;

	try
	{
		std::string toParse = http.getHtml(configuration.getWebsiteUrl());
		HTMLElement* root = parser.parse(toParse);
	}
	catch(const char* e)
	{
		std::cout << e;
	}

	return 0;
}
