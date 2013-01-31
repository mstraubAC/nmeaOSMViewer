/*
 * Tools.h
 *
 *  Created on: Jan 24, 2013
 *      Author: marcel
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <vector>
#include <string>

class Tools
{
	private:
		Tools();
		virtual ~Tools();

	public:
		static std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& dst);
		static std::vector<std::string> split(const std::string& s, char delim);
};

#endif /* TOOLS_H_ */
