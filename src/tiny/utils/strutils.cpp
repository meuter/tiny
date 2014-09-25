#include "strutils.h"
#include <boost/algorithm/string.hpp>

#include <iostream>

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

	std::string join(const std::vector<std::string> &tokens, const std::string &joiner)
	{
		return boost::algorithm::join(tokens, joiner);
	}

	std::string fileExtension(const std::string &filename)
	{
		auto splitted = split(filename, '.');
		if (splitted.size() == 1)
			return "";

		return splitted[splitted.size()-1];
	}

	std::string fileDirectory(const std::string &path, char directorySeperator)
	{
		auto sep = std::string(1, directorySeperator);
		auto splitted = split(path, directorySeperator);

		if (splitted.size() == 1)
			return "." + sep;

		splitted.pop_back();
		return join(splitted, sep) + sep;
}

}}