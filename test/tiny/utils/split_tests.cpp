#include <gtest/gtest.h>
#include <tiny/utils/split.h>

TEST(split, can_be_used_to_split_a_string)
{
	std::vector<std::string> expected{ "this", "is", "a", "test" };
	std::string toBeSplit = "this is a test";

	EXPECT_EQ(expected, tiny::utils::split(toBeSplit, ' '));
}

TEST(split, can_be_used_to_add_the_result_of_a_split_to_an_existing_vector)
{
	std::vector<std::string> expected{ "toto", "this", "is", "a", "test" };
	std::vector<std::string> actual{ "toto" };
	std::string toBeSplit = "this is a test";

	EXPECT_EQ(&actual, &tiny::utils::split(toBeSplit, ' ', actual));
	EXPECT_EQ(expected, actual);
}
