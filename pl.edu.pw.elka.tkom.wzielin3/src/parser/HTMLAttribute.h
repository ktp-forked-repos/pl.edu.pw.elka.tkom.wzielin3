/*
 * HTMLAttribute.h
 *
 *  Created on: Apr 5, 2016
 *      Author: wojciech
 */

#ifndef PARSER_HTMLATTRIBUTE_H_
#define PARSER_HTMLATTRIBUTE_H_
#include<vector>
#include<string>
class HTMLAttribute
{
public:
	HTMLAttribute();
	virtual ~HTMLAttribute();

	std::string name;
	std::vector<std::string> value;
};

#endif /* PARSER_HTMLATTRIBUTE_H_ */
