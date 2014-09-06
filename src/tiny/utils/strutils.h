#ifndef __TINY_UTILS_STRUTILS_H__
#define __TINY_UTILS_STRUTILS_H__

#include <vector>
#include <string>

namespace tiny { namespace utils {

std::string              toupper(const std::string &original);
std::vector<std::string> split(const std::string &original, const char delim=' ');

}}

#endif
