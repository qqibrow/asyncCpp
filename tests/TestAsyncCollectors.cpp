//
// Created by Lu Niu on 8/29/15.
//

#include "gtest/gtest.h"
#include "Each.h"
#include <functional>
#include <boost/type_traits.hpp>
#include <iostream>
#include <map>
#include <type_traits>

using boost::function_traits;
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

TEST_F(AsyncCollectorsTest, TestFilter) {
    vector<int> numbers = {1, 2, 4, 16, 11, 3, 15};
    Async::Filter<int> filter;
    filter(numbers, [](int number, std::function<void (bool)> callback){
        callback(number > 10);
    }, [](const vector<int>& res){
        for(const auto& s : res) {
            cout << s;
        }
    });
}

TEST_F(AsyncCollectorsTest, TestFoldl) {
    vector<int> numbers {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Async::Foldl<int> filter;
    filter(numbers, 0, [](int accumulate, int now, std::function<void(int)> callback){
        callback(accumulate + now);
    }, [](int res) {
        cout << res << endl;
    });
}



