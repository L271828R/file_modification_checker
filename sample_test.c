#include <gtest/gtest.h>

TEST(MyTest, Example) {
    int value = 42;
    ASSERT_EQ(42, value);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

