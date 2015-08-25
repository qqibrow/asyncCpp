#include "gtest/gtest.h"

class TestFoo : public ::testing::Test {
protected:
    TestFoo() {};
    virtual ~TestFoo() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(TestFoo, ShouldWork) {
EXPECT_EQ(1, 1);
}

TEST_F(TestFoo, ShouldWork2) {
    EXPECT_EQ(2, 2);
}
