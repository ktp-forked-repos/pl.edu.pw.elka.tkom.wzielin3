/*
 * ResultModel.h
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#ifndef INTERPRETER_RESULTMODEL_H_
#define INTERPRETER_RESULTMODEL_H_

#include<string>
#include<map>
#include"../consts/Enums.h"

/**
 * Objects of this class represent a single entry in the table of parsed HTML.
 */
class ResultModel
{
public:
	ResultModel();
	virtual ~ResultModel();

	std::string date;
	ThreatType threatType;
	MalwareType malwareType;
	std::string host;
};

#endif /* INTERPRETER_RESULTMODEL_H_ */
