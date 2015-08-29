#include "gtest/gtest.h"
#include "Foo.h"
#include <functional>
#include <vector>
#include <boost/function.hpp>
#include <boost/type_traits.hpp>



template <class... Ts> struct tuple {};

template <class T, class... Ts>
struct tuple<T, Ts...> : tuple<Ts...> {
    tuple(T t, Ts... ts) : tuple<Ts...>(ts...), tail(t) {}

    T tail;
};

template <bool...> struct bool_pack;

template <bool... v>
using all_true = std::is_same<bool_pack<true, v...>, bool_pack<v..., true>>;

template <typename > struct is_a_tuple : std::false_type { };
template <typename... T> struct is_a_tuple<tuple<T...>> : std::true_type { };

template <typename... Tuples>
struct VariadicTuples {
    static_assert(all_true<is_a_tuple<Tuples>::value...>::value, "!");
};



class TestFoo : public ::testing::Test {
protected:
    TestFoo() {};
    virtual ~TestFoo() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(TestFoo, ShouldWork) {
//    Functions<1,2,3,4,5> ff;
//    Functions<tuple<int, double>, tuple<double, double>> foo;
 //   VariadicTuples<int(int), double(int, int)> foo;
//    tuple<double, uint64_t, const char*> t1(12.2, 42, "big");
EXPECT_EQ(1, 1);
}

TEST_F(TestFoo, ShouldWork2) {
    EXPECT_EQ(2, 2);

}
