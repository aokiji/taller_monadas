#include <iostream>
#include <list>
#include <utility>
#include <sstream>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#define BOOST_TEST_MODULE ej
#include <boost/test/included/unit_test.hpp>

namespace std {
template<typename X, typename Y>
std::ostream& operator<<(std::ostream& os, const std::pair<X, Y>& p)
{
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}
};

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
    decltype(f(std::declval<T>())) r{};
    for (const auto & x: l) {
        auto lf = f(x);
        r.splice(r.end(), lf);
    }

    return r;
}

template<typename Pred>
auto filter(Pred p)
{
    return [=] (const auto & x) {return p(x) ? mreturn<list>(x) : list<typename std::remove_const<typename std::remove_reference<decltype(x)>::type>::type>{};};
}

std::function<bool (int)> even([] (int x) {return x % 2 == 0;});

auto ejercicio1(const list<int>& l1, const list<int>& l2) {
    return (l1 >>= [&] (const int & x) {
            return l2 >>= [&] (const int & y) {
                return mreturn<list>(x + y);
            };
        }) >>= filter(even);
}

auto ejercicio2(const list<int>& l1, const list<int>& l2) {
    return (l1 >>= filter(even)) >>= [&] (const int & x) {
            return l2 >>= [&] (const int & y) {
                return mreturn<list>(x + y);
            };
        };
}

auto ejercicio3(const list<int>& l1, const list<int>& l2) {
    return l1 >>= [&] (const int & x) {
            return (l2 >>= filter(even)) >>= [&] (const int & y) {
                return mreturn<list>(x + y);
            };
        };
}

auto ejercicio4(const list<int>& l1, const list<int>& l2) {
    return l1 >>= [&] (const int & x) {
            return l2 >>= [&] (const int & y) {
                return mreturn<list>(std::make_pair(x, y));
            };
        };
}

auto ejercicio5(const list<int>& l1, const list<int>& l2) {
    return l1 >>= [&] (const int & x) {
            return l2 >>= [&] (const int & y) {
                return mreturn<list>(std::make_pair(x, y)) >>= filter(
                        [&] (const auto & z) {return even(x) || even(y);});
            } ;
        };
}


BOOST_AUTO_TEST_SUITE( ej )
BOOST_AUTO_TEST_CASE( test )
{
    BOOST_CHECK_EQUAL(to_string(ejercicio1(list<int>{0, 1, 2}, list<int>{1, 2, 3})), "[2 2 4 4]");
    BOOST_CHECK_EQUAL(to_string(ejercicio2(list<int>{0, 1, 2}, list<int>{1, 2, 3})), "[1 2 3 3 4 5]");
    BOOST_CHECK_EQUAL(to_string(ejercicio3(list<int>{0, 1, 2}, list<int>{1, 2, 3})), "[2 3 4]");
    BOOST_CHECK_EQUAL(to_string(ejercicio4(list<int>{0, 1, 2}, list<int>{1, 2, 3})), "[(0, 1) (0, 2) (0, 3) (1, 1) (1, 2) (1, 3) (2, 1) (2, 2) (2, 3)]");
    BOOST_CHECK_EQUAL(to_string(ejercicio5(list<int>{0, 1, 2}, list<int>{1, 2, 3})), "[(0, 1) (0, 2) (0, 3) (1, 2) (2, 1) (2, 2) (2, 3)]");
}
BOOST_AUTO_TEST_SUITE_END()
