/*
 * HttpService.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: wojciech
 */

#include "HttpService.h"
#include <fstream>
#include <iostream>
HttpService::HttpService()
{
	// TODO Auto-generated constructor stub

}

HttpService::~HttpService()
{
	// TODO Auto-generated destructor stub
}

std::string HttpService::getHtml(std::string url)
{
	std::ifstream stream;
	int length;
	stream.open(url.c_str());      // open input file
	stream.seekg(0, std::ios::end);    // go to the end
	length = stream.tellg();           // report location (this is the length)
	stream.seekg(0, std::ios::beg);    // go back to the beginning
	char* buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
	stream.read(buffer, length);       // read the whole file into the buffer
	stream.close();                    // close file handle
	std::string returnString(buffer);
	delete buffer;
	return returnString;
}
