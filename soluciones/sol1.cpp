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

optional<double> ias(double x)
{
    auto r1 = sqr(x);
    auto r2 = r1 ? arcsin(r1.get()) : none;
    return r2 ? inv(r2.get()) : none;
}

BOOST_AUTO_TEST_SUITE( ej1 )
BOOST_AUTO_TEST_CASE( test )
{
    BOOST_CHECK_EQUAL(ias(-1), none);
    BOOST_CHECK_EQUAL(ias(4), none);
    BOOST_CHECK_CLOSE(ias(0.75).get(), 0.954929658, 1e-7);
}
BOOST_AUTO_TEST_SUITE_END()
