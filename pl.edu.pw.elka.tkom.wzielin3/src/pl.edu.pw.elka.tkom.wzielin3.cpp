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
#include "interpreter/HTMLInterpreter.h"

int main() {
	ConfigurationManager configuration;
	HttpService http;
	HTMLElement root;

	try
	{
		std::string toParse = http.getHtml(configuration.getWebsiteUrl());

		HTMLParser parser(toParse, 0, &root);
		parser.parse();

		HTMLInterpreter interpreter(&root);
		std::vector<ResultModel*> models = interpreter.interpret();

		std::cout<<"Finished";
	}
	catch(const char* e)
	{
		std::cout << e;
	}

	return 0;
}
