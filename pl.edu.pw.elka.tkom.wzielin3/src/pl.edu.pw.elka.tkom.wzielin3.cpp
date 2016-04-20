//============================================================================
// Name        : wzielin3.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "configuration/ConfigurationManager.h"
#include "interpreter/HTMLInterpreter.h"
#include "filter/ModelsFilter.h"
#include "http/Http.h"
#include "log/ConsoleLog.h"
#include "parser/Parser.h"
#include "lexer/Lexer.h"

int main(int argc, char** argv)
{
	ConfigurationManager configuration(argc, argv);

	Http http;
	std::string toParse = http.getHtml(configuration.webSiteUrl);

	Lexer lexer(toParse);

	HTMLElement root;
	Parser parser(&lexer, &root);
	parser.parse();

	HTMLInterpreter interpreter(&root);
	std::vector<ResultModel*> allModels = interpreter.interpret();

	ModelsFilter filter(allModels);
	if (configuration.applyMalwareFilter)
	{
		filter.applyMalwareTypeFilter(configuration.malwareType);
	}
	if (configuration.applyThreatFilter)
	{
		filter.applyThreatTypeFilter((configuration.threaType));
	}

	std::vector<ResultModel*> filteredModels = filter.getModels();

	ConsoleLog log;
	log.logResults(filteredModels);

	return 0;
}
