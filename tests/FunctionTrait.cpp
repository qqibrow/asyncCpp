//
// Created by Lu Niu on 8/26/15.
//

#include "gtest/gtest.h"
#include <functional>
#include <sstream>
#include <boost/type_traits.hpp>
#include <boost/tuple/tuple.hpp>

using std::function;

class TestFunctionTraits : public ::testing::Test {
protected:
    TestFunctionTraits() {};
    virtual ~TestFunctionTraits() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

using boost::function_traits;

template<typename F1, typename F2>
struct Curryable :
        std::integral_constant<bool, std::is_same<typename function_traits<F1>::result_type, typename function_traits<F2>::arg1_type>::value> {
};



TEST_F(TestFunctionTraits, Curryable) {
    Curryable<int(double), double(int)> c;
    //static_assert(std::is_same<function_traits<long (int, void(int))>::arg2_type, void(int)>::value, "!");
    //std::cout << std::is_same< function_traits<int(int)>::result_type, function_traits<int(int)>::arg1_type >::value << std::endl;
}


/*
 * Check whether a series of functions are channable.
 */

using namespace ::boost::tuples;

template<typename... Ts>
struct isCurryable {};

template<typename T>
struct isCurryable<T> : std::true_type {};

template<typename T, typename... Ts>
struct isCurryable<T, Ts...> : std::integral_constant<bool, Curryable<T, typename tuple<Ts...>::head_type>::value && isCurryable<Ts...>::value> {};


static_assert(Curryable<int(double), int(int)>::value, "!");
static_assert(!Curryable<int(double), int(double)>::value, "!");

static_assert(isCurryable<int(int)>::value, "!");
static_assert(isCurryable<int(double), int(int)>::value, "!");
static_assert(!isCurryable<int(double), int(double)>::value, "!");
static_assert(isCurryable<tuple<int,double> (double), std::string(tuple<int, double>), float(std::string)>::value, "!");

/*
 * check functionfuncitontrait
 */

template<typename T>
struct FunctionFunctionTrait;

template<typename T>
struct FunctionFunctionTrait<std::function<T>> {
    typedef function_traits<T> F;
};

static_assert(std::is_same<FunctionFunctionTrait< std::function<void (int)> >::F::result_type , void>::value, "!");

/*
 *  do some realwork here
 */


template<typename... Ts>
struct Waterfall {};


// T = void(std::function<something>)
// arg1_type = int
// arg2_type = std::function<>
// callback only accept void(T) type
template<typename T>
struct Waterfall<T> {
    Waterfall(std::function<T> t): func(t) {}
    using InputType = typename function_traits<T>::arg1_type;
    using Callback = typename function_traits<T>::arg2_type;
    using PassType = typename FunctionFunctionTrait<Callback>::F::arg1_type;
        void operator()(const InputType& input, std::function<void(PassType)> cb) {
            func(input, cb);
        }
    std::function<T> func;
};

// T = void( input, callback=std::function<
template<typename T, typename... Ts>
struct Waterfall<T, Ts...> {
    Waterfall(std::function<T> t, Waterfall<Ts...> ts): func(t), rest(ts) {}
    //using firstInRest = typename tuple<Ts...>::head_type;
    using InputType = typename function_traits<T>::arg1_type;
    using Callback = typename function_traits<T>::arg2_type;
    using PassType = typename FunctionFunctionTrait<Callback>::F::arg1_type;
    using LastType = typename FunctionFunctionTrait< typename function_traits< typename tuple<Ts...>::tail_type >::arg2_type >::F::arg1_type;
    void operator()(const InputType& input, std::function<void(std::string)> cb) {
        auto callback = [&](PassType pass) {
            rest(pass);
        };
        func(input, callback);
    }
    std::function<T> func;
    Waterfall<Ts...> rest;
};

/*

template<typename T, typename... Ts>
struct Waterfall {
    using next_func = typename tuple<Ts...>::head_type;
    void operator()(){

    }
};
*/

using std::string;
using std::cout;
TEST_F(TestFunctionTraits, TestOneParam) {
    Waterfall<void (int, std::function<void (string)> ) > testOneParam(
            [](int a, std::function<void(string)> cb) {
                std::stringstream s;
                s << a;
                cb(s.str());
            }
    );
    testOneParam(5, [](string s) {
        cout << s;
    });
   // Waterfall< void (int, std::function< void (string)>), void (string, std::function<void(double)>), void (double, std::function<void (long)>)>
     //       waterfall;
}