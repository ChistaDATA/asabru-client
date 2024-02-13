#include <gtest/gtest.h>
#include <string>
// Demonstrate some basic assertions.
TEST(UtilsTest, Split) {
    std::string example = "Hello,World";
    EXPECT_STREQ(example.c_str(), "Hello,World");
}