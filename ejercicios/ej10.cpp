#include <iostream>
#include <functional>
#define BOOST_TEST_MODULE ej
#include <boost/test/included/unit_test.hpp>

template<typename T>
class cont
{
public:
    cont(const T& x):run_([=](){return x;}){}

    template<typename F>
    cont(const cont& c,F f):run_([=](){return f(c.run()).run();}){}

    T run()const{return run_();}
private:
    std::function<T()> run_;
};

template<template<typename> class M,typename T>
M<T> mreturn(const T& x)
{
  return x;
}

template<typename T,typename F>
auto operator>>=(const cont<T>& c, F f)
{
    return cont<T>{c, f};
}

#define DO(var,monad,body)       \
((monad)>>=[=](const auto& var){ \
  return body;                   \
})

cont<int> fac(int n)
{
    /* fill */
}

BOOST_AUTO_TEST_SUITE( ej )
BOOST_AUTO_TEST_CASE( test )
{
  BOOST_CHECK_EQUAL(fac(5).run(), 120);
}
BOOST_AUTO_TEST_SUITE_END()
