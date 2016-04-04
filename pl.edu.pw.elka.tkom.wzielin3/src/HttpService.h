/*
 * HttpService.h
 *
 *  Created on: Apr 4, 2016
 *      Author: wojciech
 */

#ifndef CURL_HTTPSERVICE_H_
#define CURL_HTTPSERVICE_H_

#include <iostream>

class HttpService
{
public:
	HttpService();
	virtual ~HttpService();

	std::string getHtml(std::string url);
};

#endif /* CURL_HTTPSERVICE_H_ */
