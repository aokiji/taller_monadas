#include <boost/optional.hpp>
#include <iostream>
#include <cmath>
#define BOOST_TEST_MODULE ej1
#include <boost/test/included/unit_test.hpp>

template<typename T>
std::ostream& operator<<(std::ostream& os,const boost::optional<T>& x)
{
  if(x)return os<<x.get();
  else return os<<"none";
}

using namespace boost;

optional<double> inv(double x)
{
  if(x==0.0)return none;
  else      return 1.0/x;
}

optional<double> sqr(double x)
{
  if(x<0.0)return none;
  else     return std::sqrt(x);
}

optional<double> arcsin(double x)
{
  if(x<-1.0||x>1.0)return none;
  else             return std::asin(x);
}

template<template<typename> class M, typename T>
M<T> mreturn(const T &x)
{
    return x;
}

/* General definition*/
template<template<typename> class M, typename T, typename F>
auto operator>>=(const M<T> & m, F f) -> decltype(f(std::declval<const T>()));

/* Overload */
template<typename T, typename F>
auto operator>>=(const optional<T> &x, F f) -> decltype(f(std::declval<const T>())) 
{
    return x ? f(x.get()) : none;
}

optional<double> ias(double x)
{
    return (((mreturn<optional>(x) >>= sqr) >>= arcsin) >>= inv);
}

BOOST_AUTO_TEST_SUITE( ej1 )
BOOST_AUTO_TEST_CASE( test )
{
    BOOST_CHECK_EQUAL(ias(-1), none);
    BOOST_CHECK_EQUAL(ias(4), none);
    BOOST_CHECK_CLOSE(ias(0.75).get(), 0.954929658, 1e-07);
    BOOST_CHECK_CLOSE(ias(0.25).get(), 1.909859317, 1e-07);
}
BOOST_AUTO_TEST_SUITE_END()
