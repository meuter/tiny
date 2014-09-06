#include <gtest/gtest.h>
#include <tiny/utils/strutils.h>

TEST(toupper, can_be_used_to_make_a_string_in_upper_case)
{
	EXPECT_EQ("OBJ", tiny::utils::toupper("obJ"));
}

TEST(split, can_be_used_to_split_a_string)
{

	std::vector<std::string> expected{ "this", "is", "a", "test" };
	std::string toBeSplit = "this is a test";

	EXPECT_EQ(expected, tiny::utils::split(toBeSplit, ' '));
}
