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
#include "interpreter-details/DetailsInterpreter.h"
#include "filter/ModelsFilter.h"
#include "http/Http.h"
#include "log/ConsoleLog.h"
#include "parser/Parser.h"
#include "lexer/Lexer.h"
#include "file/File.h"

int main(int argc, char** argv)
{
	ConfigurationManager configuration(argc, argv);

	std::string toParse =
			configuration.readFromFile ?
					File().getHtml(configuration.path) :
					Http().getHtml(configuration.path);

	Lexer lexer(toParse);
	HTMLElement root;
	Parser(&lexer, &root).parse();

	if(configuration.single)
	{
		DetailsInterpreter interpreter(&root);
		DetailsResultModel model = interpreter.interpret();
		ConsoleLog().logResult(model);
	}
	else
	{
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
		ConsoleLog().logResults(filteredModels);
	}
	return 0;
}
