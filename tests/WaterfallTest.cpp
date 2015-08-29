//
// Created by Lu Niu on 8/28/15.
//

#include "gtest/gtest.h"
#include <functional>
#include <sstream>
#include <boost/type_traits.hpp>
#include <boost/tuple/tuple.hpp>

using std::function;
using boost::function_traits;
using namespace std;
using namespace ::boost::tuples;

class WaterfallTest : public ::testing::Test {
protected:
    WaterfallTest() {};
    virtual ~WaterfallTest() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};


template<typename T>
struct Callback {
    typedef function< void(T) > f;
};

template<typename Intype, typename OType>
struct CallbackStruct {
    typedef function< void(Intype, typename Callback<OType>::f) > f;
};

template<typename... Ts>
struct Waterfall {};


template<typename A, typename B>
struct Waterfall<A, B> {
    Waterfall(typename CallbackStruct<A, B>::f func): func_(func) {}
    void operator()(A input, typename Callback<B>::f cb) {
        func_(input, cb);
    }
    typename CallbackStruct<A,B>::f func_;
};

/*

template<typename A, typename B, typename C>
struct Waterfall<A, B, C> {
    Waterfall(typename CallbackStruct<A, B>::f func1, typename CallbackStruct<B, C>::f func2): func1_(func1), func2_(func2) {}
    void operator()(A input, typename Callback<C>::f cb) {
        auto func1to2 = [&](B b) {
            func2_(b, cb);
        };
        func1_(input, func1to2);
    }
    typename CallbackStruct<A, B>::f func1_;
    typename CallbackStruct<B, C>::f func2_;
};
 */

template<typename T, typename... Ts>
struct Waterfall<T, Ts...> {
    using HeadType = typename boost::tuple<Ts...>::head_type;
    using TailType = typename std::tuple_element<std::tuple_size<std::tuple<Ts...>>::value-1, std::tuple<Ts...>>::type;

    Waterfall(typename CallbackStruct<T, HeadType>::f func, Waterfall<Ts...> rest): func_(func), rest_(rest) {}
    void operator()(T input, typename Callback<TailType>::f cb) {
        auto passToRest = [&](HeadType pass) {
            rest(pass,cb);
        };
        func_(input, passToRest);
    }
    typename CallbackStruct<T, HeadType>::f func_;
    Waterfall<Ts...> rest_;
};

TEST_F(WaterfallTest, TestWaterfallTwo) {
    Waterfall<int, string> twoparams([](int a, Callback<string>::f callback) {
        std::stringstream s;
        s << a;
        callback(s.str());
    });
    twoparams(121, [](string s) {
        cout << s << endl;
    });
}

TEST_F(WaterfallTest, TestWaterfallThree) {
    Waterfall<int, string, string> threeparams(
            [](int a, Callback<string>::f callback) {
                std::stringstream s;
                s << a;
                callback(s.str());
            },
            Waterfall<string, string>(
                    [](string s, Callback<string>::f callback) {
                        callback(s + " hahahha");
                    }
            )
    );
    /*
    threeparams(1234, [](string s){
        cout << s << endl;
    });
     */
}