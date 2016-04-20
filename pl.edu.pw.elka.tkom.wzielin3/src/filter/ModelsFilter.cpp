/*
 * ModelsFilter.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "ModelsFilter.h"

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

void ModelsFilter::applyThreatTypeFilter(ThreatType type)
{
	std::vector<ResultModel*> newModels;
	for (unsigned int i = 0; i < models.size(); ++i)
	{
		if (models[i]->threatType == type)
		{
			newModels.push_back(models[i]);
		}
	}
	models = newModels;
}


void ModelsFilter::applyMalwareTypeFilter(MalwareType type)
{
	std::vector<ResultModel*> newModels;
	for (unsigned int i = 0; i < models.size(); ++i)
	{
		if (models[i]->malwareType == type)
		{
			newModels.push_back(models[i]);
		}
	}
	models = newModels;
}
