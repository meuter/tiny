#ifndef __TINY_UTILS_STRUTILS_H__
#define __TINY_UTILS_STRUTILS_H__

#include <vector>
#include <string>

namespace tiny { namespace utils {

std::string              toupper(const std::string &original);
std::vector<std::string> split(const std::string &original, const char delim=' ');
std::string              join(const std::vector<std::string> &tokens, const std::string &joiner="");

std::string fileExtension(const std::string &filename);
std::string fileDirectory(const std::string &path, char directorySeperator='/');

}}

#endif
