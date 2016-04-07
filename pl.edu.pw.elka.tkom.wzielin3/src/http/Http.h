/*
 * HttpService.h
 *
 *  Created on: Apr 4, 2016
 *      Author: wojciech
 */

#ifndef CURL_HTTPSERVICE_H_
#define CURL_HTTPSERVICE_H_

#include <iostream>

class Http
{
public:
	Http();
	virtual ~Http();

	std::string getHtml(std::string url);
};

#endif /* CURL_HTTPSERVICE_H_ */
