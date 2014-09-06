#ifndef __TINY_UTILS_UPPER_H__
#define __TINY_UTILS_UPPER_H__

#include <string>

namespace tiny { namespace utils {

std::string toupper(const std::string &s)
{
	std::string result;

	for (char c: s)
		result.append(1, std::toupper(c));

	return result;
}

}}

#endif