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

template<typename T>
struct Callback {
    typedef std::function< void (std::function<void(T)>) > f;
};

struct Error {};

template<typename... Ts>
struct ErrorCallback;

template <typename T>
struct ErrorCallback<T> {
    typedef std::function<void(Error, T t)> f;
};

template<typename... Ts>
struct ErrorCallback {
    typedef std::function<void(Error, Ts... ts)> f;
};

template<typename... Ts>
struct series {};


template<typename T>
struct series<T> {
    void operator()(typename Callback<T>::f func, typename ErrorCallback<T>::f callback) {

    }
};

template<typename T1, typename T2>
struct series<T1, T2> {
    void operator()(typename Callback<T1>::f func1, typename Callback<T2>::f func2, typename ErrorCallback<T1, T2>::f callback) {

    }
};

TEST_F(AsyncCollectorsTest, TestSeries) {
    series<int> series1;
    series1([](std::function<void(int)> callback) {

    }, [](Error e, int res) {

    });

    series<int, string> series2;
    series2([](std::function<void(int)> callback) {

    }, [](std::function<void(string)> callback){

    }, [](Error e, int res1, string res2) {

    });
}


template <typename T>
struct Trait2;

template <typename R>
struct Trait2<std::function< void(std::function<void(R)>)> > {
    typedef R RETURN_TYPE;

};

template <typename T>
struct Trait;

template <typename R>
struct Trait<std::function<void(R)>> {
    typedef R ARG_TYPE;

};

static_assert(std::is_same<Trait<std::function<void(int)>>::ARG_TYPE, int>::value, "int and int is same type");
static_assert(!std::is_same<Trait<std::function<void(int)>>::ARG_TYPE, std::string>::value, "int and string is not");

template <typename T>
struct lambda_traits
        : public lambda_traits<decltype(&T::operator())>
{};
// For generic types, directly use the result of the signature of its 'operator()'

template <typename ClassType, typename ReturnType, typename... Args>
struct lambda_traits<ReturnType(ClassType::*)(Args...) const>
// we specialize for pointers to member function
{
    enum { arity = sizeof...(Args) };
    // arity is the number of arguments.

    typedef ReturnType result_type;

    template <size_t i>
    struct arg
    {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
        // the i-th argument is equivalent to the i-th tuple element of a tuple
        // composed of those arguments.
    };
};

struct doFunc {
    template<typename T>
    void operator()(T func) {
        typedef typename lambda_traits<decltype(func)>::template arg<0>::type TTT;
        typedef typename Trait<TTT>::ARG_TYPE RETURN_TYPE;
        auto f = [](RETURN_TYPE a) {
            cout << a;
        };
        func(f);
    }
};

#include "ArgPackHelper.h"
template<typename... Ts>
void func(Ts&&... args) {
    for_each_in_arg_pack(doFunc(), forward<Ts>(args)...);
}

TEST_F(AsyncCollectorsTest, TestTest) {

    func(
            [](std::function<void(string)> cb) {
                cb("hello");
            },
            [](std::function<void(int)> cb) {
                cb(5.0);
            }
    );
    doFunc()(
            [](std::function<void(string)> cb) {
                cb("hello");
            }
    );
}
