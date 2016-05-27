/*
 * ModelsFilter.h
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#ifndef FILTER_MODELSFILTER_H_
#define FILTER_MODELSFILTER_H_

#include<vector>
#include"../interpreter/ResultModel.h"

class ModelsFilter
{
public:
	/**
	 * Default Constructor for filter class.
	 * @param models that are to be stored in this filter.
	 */
	ModelsFilter(std::vector<ResultModel*> models);
	virtual ~ModelsFilter();

	/**
	 * Method used for retrieving filtered models.
	 * @returns filtered models
	 */
	std::vector<ResultModel*> getModels();

	/**
	 * Method that applies filter based on ThreatType to models stored in this filter.
	 * @param threat ThreatType that has to match for a model to pass filter
	 */
	void applyThreatTypeFilter(std::string threat);


	/**
	 * Method that applies filter based on MalwareType to models stored in this filter.
	 * @param malware MalwareType that has to match for a model to pass filter
	 */
	void applyMalwareTypeFilter(std::string malware);

private:
	std::vector<ResultModel*> models;
	bool applyThreatFilter;
	bool applyMalwareFilter;
};

#endif /* FILTER_MODELSFILTER_H_ */
