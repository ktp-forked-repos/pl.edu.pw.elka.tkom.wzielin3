/*
 * Utils.cpp
 *
 *  Created on: May 27, 2016
 *      Author: wojciech
 */

#include "Utils.h"
#include <algorithm>

bool Utils::stringsEqual(std::string a, std::string b)
{
	std::transform(a.begin(), a.end(), a.begin(), ::tolower);
	std::transform(b.begin(), b.end(), b.begin(), ::tolower);
	return a == b;
}
