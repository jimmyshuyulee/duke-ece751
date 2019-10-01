#include <cstdlib>
#include <iostream>
#include <map>
using namespace std;

template<typename Num>
class Polynomial {
  map<int, Num> coeff;

 public:
  // Default construct the polynomial to be "0" (i.e., 0*x^0)
  Polynomial() : coeff() { coeff[0] = Num(); }

  // Add this Polynomial to rhs, and return the resulting polynomial.
  // For example if this is 2 * x ^ 2 + 3, and rhs is x ^ 2 + 4 * x,
  // you should return the Polynomial 3 * x ^ 2 + 4 * x + 3.
  Polynomial operator+(const Polynomial & rhs) const {
    Polynomial<Num> ans = Polynomial<Num>(*this);

    typename map<int, Num>::const_iterator itr = rhs.coeff.begin();
    while (itr != rhs.coeff.end()) {
      // Add the coefficients in rhs to ans if the term exist.
      // Otherwise, insert it into ans.
      if (ans.coeff.find(itr->first) == ans.coeff.end()) {
        ans.coeff.insert(*itr);
      }
      else {
        ans.coeff[itr->first] = ans.coeff[itr->first] + itr->second;
      }
      ++itr;
    }
    return ans;
  }

  // Return the negation of this Polynomial.
  // For example, if this Polynomial is 4 * x ^ 3 - 2, return -4 * x ^ 3 + 2
  Polynomial operator-() const {
    Polynomial<Num> ans = Polynomial<Num>(*this);
    typename map<int, Num>::iterator itr = ans.coeff.begin();
    while (itr != ans.coeff.end()) {
      itr->second = -(itr->second);
      ++itr;
    }
    return ans;
  }

  // Subtract rhs from this Polynomial and return the result
  Polynomial operator-(const Polynomial & rhs) const {
    Polynomial<Num> ans = Polynomial<Num>(rhs);
    return *this + (-ans);
  }

  // Multiply this Polynomial by a scalar and return the result
  Polynomial operator*(const Num & n) const {
    Polynomial<Num> ans = Polynomial<Num>(*this);
    typename map<int, Num>::iterator itr = ans.coeff.begin();
    while (itr != ans.coeff.end()) {
      itr->second = n * itr->second;
      ++itr;
    }
    return ans;
  }

  // Multiply this Polynomial by rhs, and return the result.
  Polynomial operator*(const Polynomial & rhs) const {
    Polynomial<Num> ans = Polynomial<Num>();
    typename map<int, Num>::const_iterator itr1 = coeff.begin();
    while (itr1 != coeff.end()) {
      typename map<int, Num>::const_iterator itr2 = rhs.coeff.begin();
      while (itr2 != rhs.coeff.end()) {
        ans.addTerm(itr1->second * itr2->second, itr1->first + itr2->first);
        ++itr2;
      }
      ++itr1;
    }
    return ans;
  }

  // Compare two Polynomials for equality and return the result.
  bool operator==(const Polynomial & rhs) const {
    typename map<int, Num>::const_iterator itr = coeff.begin();
    while (itr != coeff.end()) {
      if (rhs.coeff.find(itr->first) == rhs.coeff.end()) {
        return false;
      }
      // Already checked that itr->first is a key in rhs.coeff,
      // so the at() function is no exception guaranteed
      if (rhs.coeff.at(itr->first) != itr->second) {
        return false;
      }
      ++itr;
    }
    return true;
  }

  // Compare two Polynomials for inequality and return the result.
  bool operator!=(const Polynomial & rhs) const { return !(*this == rhs); }

  // Add (c*x^p) to this Polynomial, updatings its value.  For example,
  void addTerm(const Num & c, int p) {
    if (coeff.find(p) != coeff.end()) {
      coeff[p] = coeff[p] + c;
    }
    else {
      coeff[p] = c;
    }
  }

  // Perform the + operation but update this Polynomial,
  // instead of returning a different Polynomial.
  Polynomial & operator+=(const Polynomial & rhs) {
    *this = *this + rhs;
    return (*this);
  }

  // Perform the - operation but update this Polynomial,
  // instead of returning a different Polynomial.
  Polynomial & operator-=(const Polynomial & rhs) {
    *this = *this - rhs;
    return (*this);
  }

  // Perform the * operation but update this Polynomial,
  // instead of returning a different Polynomial.
  Polynomial & operator*=(const Polynomial & rhs) {
    *this = *this * rhs;
    return (*this);
  }

  // Perform the * operation but update this Polynomial,
  // instead of returning a different Polynomial.
  Polynomial & operator*=(const Num & rhs) {
    *this = *this * rhs;
    return (*this);
  }

  //This evaluates the Polynomial at the given value of "x", and returns that answer.
  Num eval(const Num & x) const {
    Num ans = Num();
    typename map<int, Num>::const_iterator itr = coeff.end();
    while (--itr != coeff.begin()) {
      Num term = itr->second;

      // Avoid using +=, *= or pow() here since it is not guaranteed
      // that Num will have these operators or functions.
      for (int i = 0; i < itr->first; ++i) {
        term = term * x;
      }
      ans = ans + term;
    }
    ans = ans + itr->second;
    return ans;
  }

  Polynomial derivative() const {
    Polynomial<Num> ans = Polynomial<Num>();
    typename map<int, Num>::const_iterator itr = coeff.begin();
    while (itr != coeff.end()) {
      if (itr->first > 0) {
        ans.addTerm(itr->second * itr->first, itr->first - 1);
      }
      ++itr;
    }
    return ans;
  }

  template<typename N>
  friend std::ostream & operator<<(std::ostream & os, const Polynomial<N> & p);
};

template<typename N>
// This should write the Polynomial to "os" such that it looks like this:
// "4*x^4 + 2*x^3 + -7*x^2 + 0*x^1 + -9*x^0"
std::ostream & operator<<(std::ostream & os, const Polynomial<N> & p) {
  typename map<int, N>::const_iterator itr = p.coeff.end();
  // Keys are sorted in ascending order, so start printing from the end
  while (--itr != p.coeff.begin()) {
    os << itr->second << "*x^" << itr->first;
    os << " + ";
  }
  os << itr->second << "*x^" << itr->first;
  return os;
}
