/*
 * File.h
 *
 *  Created on: Apr 21, 2016
 *      Author: wojciech
 */

#ifndef FILE_FILE_H_
#define FILE_FILE_H_

class File
{
public:
	File();
	virtual ~File();

	std::string getHtml(std::string path);
};

#endif /* FILE_FILE_H_ */
