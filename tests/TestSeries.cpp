//
// Created by Lu Niu on 8/30/15.
//
#include "gtest/gtest.h"
#include "series.h"

class TestSeries : public ::testing::Test {
protected:
    TestSeries() { };

    virtual ~TestSeries() { }

    virtual void SetUp() { }

    virtual void TearDown() { }
};

TEST_F(TestSeries, TestTest) {

    // first check doFunc is working.
    workflow::doFunc()(
            [](std::function<void(string)> cb) {
                cb("hello");
            }
    );
    // check series is working.
    workflow::func(
            [](std::function<void(string)> cb) {
                cb("hello");
            },
            [](std::function<void(int)> cb) {
                cb(5.0);
            }
    );
}


