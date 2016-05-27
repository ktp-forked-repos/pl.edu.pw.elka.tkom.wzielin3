/*
 * ModelsFilter.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "ModelsFilter.h"
#include "../common/Utils.h"

ModelsFilter::ModelsFilter(std::vector<ResultModel*> models)
{
	this->models = models;
	applyMalwareFilter = false;
	applyThreatFilter = false;
}

ModelsFilter::~ModelsFilter()
{
}

std::vector<ResultModel*> ModelsFilter::getModels()
{
	return models;
}

void ModelsFilter::applyThreatTypeFilter(std::string threat)
{
	std::vector<ResultModel*> newModels;
	for (unsigned int i = 0; i < models.size(); ++i)
	{
		if (Utils::stringsEqual(models[i]->threatType, threat))
		{
			newModels.push_back(models[i]);
		}
	}
	models = newModels;
}


void ModelsFilter::applyMalwareTypeFilter(std::string malware)
{
	std::vector<ResultModel*> newModels;
	for (unsigned int i = 0; i < models.size(); ++i)
	{
		if (Utils::stringsEqual(models[i]->malwareType, malware))
		{
			newModels.push_back(models[i]);
		}
	}
	models = newModels;
}
