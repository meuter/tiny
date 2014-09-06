#include <gtest/gtest.h>
#include <tiny/utils/upper.h>

TEST(toupper, can_be_used_to_make_a_string_in_upper_case)
{
	EXPECT_EQ("OBJ", tiny::utils::toupper("obJ"));
}