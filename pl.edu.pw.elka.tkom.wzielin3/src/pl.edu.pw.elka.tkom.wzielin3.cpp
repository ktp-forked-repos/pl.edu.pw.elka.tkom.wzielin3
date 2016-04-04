//============================================================================
// Name        : wzielin3.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ConfigurationManager.h"
#include "HttpService.h"

int main() {
	ConfigurationManager configuration;
	HttpService http;
	http.getHtml(configuration.getWebsiteUrl());
	return 0;
}
