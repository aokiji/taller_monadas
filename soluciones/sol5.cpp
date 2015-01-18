#include <iostream>
#include <list>
#include <utility>
#include <sstream>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#define BOOST_TEST_MODULE ej
#include <boost/test/included/unit_test.hpp>

template<typename T>
std::ostream& operator<<(std::ostream& os,const std::list<T>& l)
{
  os << "[" << boost::algorithm::join(l | boost::adaptors::transformed(boost::lexical_cast<std::string, T>), " ") << "]";
  return os;
}

template<typename T>
std::string to_string(const std::list<T> & l) 
{
    std::ostringstream ss;
    ss << l;
    return ss.str();
}

template<typename T>
struct list:std::list<T>
{
  using base=std::list<T>;
  using base::base;
  list(const T& x):base(1,x){} // compatibility with mreturn
};

list<int> decinc(int x)
{
  return {x-1,x+1};
}

template<template<typename> class M, typename T>
M<T> mreturn(const T &x)
{
    return x;
}

template<typename T, typename F>
auto operator>>=(const list<T>& l, F f)
{
    list<T> r{};
    for (const auto & x: l) {
        auto lf = f(x);
        r.splice(r.end(), lf);
    }

    return r;
}


BOOST_AUTO_TEST_SUITE( ej )
BOOST_AUTO_TEST_CASE( test )
{
    BOOST_CHECK_EQUAL(to_string((decinc(3)>>=decinc)>>=decinc), "[0 2 2 4 2 4 4 6]");
    BOOST_CHECK_EQUAL(to_string(list<int>{0,1,4,5}>>=decinc), "[-1 1 0 2 3 5 4 6]");
}
BOOST_AUTO_TEST_SUITE_END()
