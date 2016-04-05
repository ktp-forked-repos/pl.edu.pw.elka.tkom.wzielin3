/*
 * ConsoleUI.h
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#ifndef CONSOLE_JSONDISPLAY_H_
#define CONSOLE_JSONDISPLAY_H_

#include<vector>
#include"../interpreter/ResultModel.h"
#include"../consts/Enums.h"

class JSONDisplay
{
public:
	JSONDisplay();
	virtual ~JSONDisplay();

	/**
	 * Displays results on standard output as JSON.
	 * @param models ResultModel collection to display
	 */
	void display(std::vector<ResultModel*> models);
private:
	/**
	 * Displays single ResultModel on standard output as JSON.
	 * @param model ResultModel to display
	 */
	void display(ResultModel* model);

	static const std::map<ThreatType, std::string> threatTypeToString;
	static const std::map<MalwareType, std::string> malwareTypeToString;
};

#endif /* CONSOLE_JSONDISPLAY_H_ */
