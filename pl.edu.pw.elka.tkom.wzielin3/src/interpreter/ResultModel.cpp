/*
 * ResultModel.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#include "ResultModel.h"

ResultModel::ResultModel()
{
	//mock default values
	threatType = ThreatType::BotnetCC;
	malwareType = MalwareType::CTBLocker;
}

ResultModel::~ResultModel()
{
}
