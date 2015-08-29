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
    using TailType = typename std::tuple_element<std::tuple_size<std::tuple<Ts...>>::value - 1, std::tuple<Ts...>>::type;

    Waterfall(typename CallbackStruct<T, HeadType>::f func, Waterfall<Ts...> rest): func_(func), rest_(rest) {}

    void operator()(T input, typename Callback<TailType>::f cb) {
        auto passToRest = [&](HeadType pass) {
            rest_(pass,cb);
        };
        func_(input, passToRest);
    }
    typename CallbackStruct<T, HeadType>::f func_;
    Waterfall<Ts...> rest_;
};

// 1 Waterfall<A,B>(f1)
// 2 Waterfall<A,B,C>(f1, Waterfall<B,C>(f2)
// 3 Waterfall<A,B,C,D>(f1, Waterfall<B,C,D>(f2, Waterfall<C,D>(f3))
#define WATERFALL_2(T1,T2,f1) Waterfall<T1, T2>(f1)
#define WATERFALL_3(T1,T2,T3,f1,f2)  Waterfall<T1,T2,T3>(f1, WATERFALL_2(T2,T3,f2))
#define WATERFALL_4(T1,T2,T3,T4,f1,f2,f3)  Waterfall<T1,T2,T3,T4>(f1,WATERFALL_3(T2,T3,T4,f2,f3))
#define WATERFALL_5(T1,T2,T3,T4,T5,f1,f2,f3,f4)  Waterfall<T1,T2,T3,T4,T5>(f1,WATERFALL_4(T2,T3,T4,T5,f2,f3,f4))

TEST_F(WaterfallTest, TestWaterfallTwo) {
    auto twoparams = WATERFALL_2(int, string,
            [](int a, Callback<string>::f callback) {
                std::stringstream s;
                s << a;
                callback(s.str());
            }
    );
    twoparams(121, [](string s) {
        cout << s << endl;
    });
}

TEST_F(WaterfallTest, TestWaterfallThree) {
    auto threeparams = WATERFALL_3(int, string, double,
            [](int a, Callback<string>::f callback) {
                std::stringstream s;
                s << a;
                callback(s.str());
            },
            [](string s, Callback<double>::f callback) {
                callback(34.56);
            }
    );
    threeparams(1234, [](double s){
        cout << s << endl;
    });

}