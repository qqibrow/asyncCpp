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

TEST_F(TestSeries, TestNewSeriesImpl) {

    // first check doFunc is working.
    workflow::DoTask()(
            [](std::function<void(string)> cb) {
                cb("hello");
            }
    );
    // check series is working.
    workflow::series(
            [](std::function<void(string)> cb) {
                cb("hello");
            },
            [](std::function<void(int)> cb) {
                cb(5.0);
            }
    );
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

TEST_F(TestSeries, TestOldSeries) {
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
