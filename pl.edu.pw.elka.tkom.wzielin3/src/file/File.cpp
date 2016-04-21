/*
 * File.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: wojciech
 */

#include <fstream>
#include "File.h"
#include "../log/ConsoleLog.h"

File::File()
{
	// TODO Auto-generated constructor stub

}

File::~File()
{
	// TODO Auto-generated destructor stub
}

std::string File::getHtml(std::string path)
{
	std::ifstream file;
	std::string s, line;
	file >> s;
	file.open(path.c_str());
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			s += line;
		}
		file.close();
	}
	else
	{
		ConsoleLog log;
		log.logError("Couldn't open file " + path + " for reading. Make sure file exisits.");
	}
	return s;
}

