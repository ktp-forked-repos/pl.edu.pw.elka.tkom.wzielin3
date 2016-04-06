/*
 * ConsoleUI.h
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#ifndef LOG_CONSOLELOG_H_
#define LOG_CONSOLELOG_H_

#include<vector>
#include"../interpreter/ResultModel.h"
#include"../consts/Enums.h"

class ConsoleLog
{
public:
	ConsoleLog();
	virtual ~ConsoleLog();

	/**
	 * Displays message on standard error output and terminates execution of program.
	 * @param message to be displayed
	 */
	void logError(std::string message);

	/**
	 * Displays results on standard output as JSON.
	 * @param models ResultModel collection to display
	 */
	void logResults(std::vector<ResultModel*> models);
private:
	/**
	 * Displays single ResultModel on standard output as JSON.
	 * @param model ResultModel to display
	 */
	void logResult(ResultModel* model);

	static const std::map<ThreatType, std::string> threatTypeToString;
	static const std::map<MalwareType, std::string> malwareTypeToString;
};

#endif /* LOG_CONSOLELOG_H_ */
