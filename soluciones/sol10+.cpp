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

cont<int> prod(int n, int m)
{
    return mreturn<cont>(n * m);
}

static int contador = 0;
cont<int> fac(int n)
{
    ++contador;
    return n == 1 ? mreturn<cont>(1) : 
        (cont<int>(n-1) >>= [] (const auto& f) {
            return fac(f);}) >>= [=] (const auto& f) {
                return prod(f, n);};
}

BOOST_AUTO_TEST_SUITE( ej )
BOOST_AUTO_TEST_CASE( test )
{
  auto f5 = fac(5);
  BOOST_CHECK_EQUAL(contador, 1);
  BOOST_CHECK_EQUAL(f5.run(), 120);
}
BOOST_AUTO_TEST_SUITE_END()
