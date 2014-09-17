#include "strutils.h"
#include <boost/algorithm/string.hpp>

namespace tiny { namespace utils {

std::string toupper(const std::string &original)
{
	return boost::to_upper_copy(original);
}

std::vector<std::string> split(const std::string &original, const char delim)
{
	std::vector<std::string> result;
	boost::split(result, original, boost::is_any_of(std::string(1, delim)));
	return result;
}

std::string fileExtension(const std::string &filename)
{
	auto splitted = split(filename, '.');
	if (splitted.size() == 1)
		return "";
	else 
		return splitted[splitted.size()-1];
}

}}