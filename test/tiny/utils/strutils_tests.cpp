#include <gtest/gtest.h>
#include <tiny/utils/strutils.h>

using namespace tiny::utils;

TEST(toupper, can_be_used_to_make_a_string_in_upper_case)
{
	EXPECT_EQ("OBJ", toupper("obJ"));
}

TEST(split, can_be_used_to_split_a_string)
{

	std::vector<std::string> expected{ "this", "is", "a", "test" };
	std::string toBeSplit = "this is a test";

	EXPECT_EQ(expected, split(toBeSplit, ' '));
}

TEST(join, can_be_used_to_join_a_vector_of_strings)
{
	std::vector<std::string> tokens = { "hello", " ", "world", "!" };
	std::vector<std::string> empty = {};
	std::vector<std::string> one = { "foo" };

	EXPECT_EQ("", join(empty));
	EXPECT_EQ("foo", join(one));
	EXPECT_EQ("hello world!", join(tokens));
	EXPECT_EQ("hello/ /world/!", join(tokens, "/"));
}

TEST(fileDirectory, can_be_used_to_extract_the_directory_path_or_a_path)
{
	EXPECT_EQ("./", fileDirectory("toto.txt"));
	EXPECT_EQ("/path/to/", fileDirectory("/path/to/toto.txt"));

	EXPECT_EQ(".\\", fileDirectory("toto.txt", '\\'));
	EXPECT_EQ("\\path\\to\\", fileDirectory("\\path\\to\\toto.txt", '\\'));

}

