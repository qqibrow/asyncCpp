//
// Created by Lu Niu on 8/29/15.
//

#include "gtest/gtest.h"
#include "Each.h"
#include <iostream>
#include <map>

class AsyncCollectorsTest : public ::testing::Test {
protected:
    AsyncCollectorsTest() {};
    virtual ~AsyncCollectorsTest() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

using namespace std;

TEST_F(AsyncCollectorsTest, TestEach) {
    vector<string> list {"abc", "def", "bar", "foo"};
    Async::Each<string> each;
    each(list, [](string s) {
        cout << s;
    });
}

TEST_F(AsyncCollectorsTest, TestForEachof) {
    std::map<int, string> m { {1, "abc"}, {2, "def"}, {3, "bar"}};
    Async::ForEachOf<std::map<int, string>> forEachOf;
    forEachOf(m, [](int key, string value) {
        cout << key << value;
    });
}

TEST_F(AsyncCollectorsTest, TestMapFunc) {
    vector<string> list {"abc", "def", "bar", "foo"};
    Async::Map<string, string> mapFunc;

    mapFunc(list, [](string input, std::function<void(string)> callback){
        callback(input + " hahahha");
    }, [](const vector<string>& list) {
        for(const auto& s : list) {
            cout << s;
        }
    });


}