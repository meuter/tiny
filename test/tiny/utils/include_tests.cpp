#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <tiny/utils/strutils.h>

std::string loadSource(const std::string &filename)
{
	static const std::regex includeMatcher("#include[\t ]+\"([a-zA-z\\.]+)\"[\t ]*");
	std::ifstream fileStream(filename.c_str());
	std::stringstream result;
	std::smatch match;
	std::string line;
	std::string path = tiny::utils::fileDirectory(filename);

	if (fileStream.fail())
		throw std::runtime_error("could not open " + filename);

	while (getline(fileStream, line))
	{
		if (regex_match(line, match, includeMatcher))
			result << loadSource(path + match[1].str());
		else			
			result << line << std::endl;
	}

	return result.str();
}

TEST(loadSource, works_without_include)
{
	auto source = loadSource("test/tiny/utils/include_test.glsl");

	std::string expected = 
"vec4 doSomeFancyCompytation()\n"
"{\n"
"	return vec4(1,1,1,1);\n"
"}\n";

	std::cout << source << std::endl;

	EXPECT_EQ(expected, source);
}

TEST(loadSource, works_with_include)
{
	auto source = loadSource("test/tiny/utils/include_test.fs");

	std::string expected = 
"#version 120\n"
"\n"
"vec4 doSomeFancyCompytation()\n"
"{\n"
"	return vec4(1,1,1,1);\n"
"}\n"
"\n"
"// #include \"commented_include.h\"\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = doSomeFancyCompytation();\n"
"}\n";

	std::cout << source << std::endl;

	EXPECT_EQ(expected, source);
}

