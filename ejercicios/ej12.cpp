#include <iostream>
#include <functional>
#include <boost/optional.hpp>
#define BOOST_TEST_MODULE ej
#include <boost/test/included/unit_test.hpp>

template<typename T>
class cont
{
public:
    /**
     * Construct with no history nor future
     */
    cont(const T& t) /* fill */ {}
    
    /**
     * Construct with base value t, and future steps given by f
     */
    template<typename F>
    cont(const T& t, F f) /* fill */  {}

    /**
     * Construct with history steps given by c, and future steps by f
     */ 
    template<typename F, typename R>
    cont(const cont<R>& c,F f) /* fill */ {}

    T run()const {
        /* run all steps */
    }

    std::pair<T,boost::optional<cont>> step()const {
        /* advance one step */
    }
private:
    /* attributes */
};

template<template<typename> class M,typename T>
M<T> mreturn(const T& x)
{
  return x;
}

template<typename T,typename F>
auto operator>>=(const cont<T>& c, F f)
{
    using M = decltype(f(std::declval<const T>()));
    return M{c, f};
}

cont<int> fac(const int x)
{
    return x == 0 ? mreturn<cont>(1) : fac(x-1) >>= [=] (const int &y) {return mreturn<cont>(y * x);};
}

BOOST_AUTO_TEST_SUITE( ej )
BOOST_AUTO_TEST_CASE( test )
{
    BOOST_CHECK_EQUAL(fac(5).run(), 120);
    auto f = fac(5).step();
    std::vector<int> steps{};
    while (f.second) {
        steps.push_back(f.first);
        f = f.second.get().step();
    }
    steps.push_back(f.first);

    std::vector<int> expected_steps {1, 1, 2, 6, 24, 120};

    BOOST_CHECK_EQUAL_COLLECTIONS(steps.begin(), steps.end(), expected_steps.begin(), expected_steps.end());
}
BOOST_AUTO_TEST_SUITE_END()
