/*
 * Tools.cpp
 *
 *  Created on: Jan 24, 2013
 *      Author: marcel
 */

#include "Tools.h"
#include <sstream>

using namespace std;

Tools::Tools()
{
	// TODO Auto-generated constructor stub

}

std::vector<std::string>& Tools::split(const std::string& s, char delim,
		std::vector<std::string>& dst)
{
	stringstream ss(s);
	string item;
	while(getline(ss, item, delim)) {
		dst.push_back(item);
	}
	return dst;
}

std::vector<std::string> Tools::split(const std::string& s, char delim)
{
	vector<string> dst;
	split(s, delim, dst);
	return dst;
}

Tools::~Tools()
{
	// TODO Auto-generated destructor stub
}

