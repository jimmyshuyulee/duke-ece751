#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

template<typename Num>
class convergence_failure : public exception {
 public:
  const Num value;

  explicit convergence_failure(const Num & v) : value(v){};
  virtual const char * what() const throw() { return "Convergence failure"; }
};

template<typename Num>
class Polynomial {
  map<int, Num, greater<int> > coeff;  // Sort the keys in decending order

 public:
  // Default construct the polynomial to be "0" (i.e., 0*x^0)
  Polynomial() : coeff() { coeff[0] = Num(); }

  // Add this Polynomial to rhs, and return the resulting polynomial.
  // This can be done elegantly by using .find() fuction to access terms in lhs,
  // but the overall time complexity will become O(nlogn) since searching in
  // map takes O(logn) time. I choose this O(n) approach which access the terms
  // of lhs by an iterator.
  Polynomial operator+(const Polynomial & rhs) const {
    Polynomial<Num> ans(*this);
    typename map<int, Num>::iterator ans_itr = ans.coeff.begin();
    typename map<int, Num>::const_iterator itr;

    for (itr = rhs.coeff.begin(); itr != rhs.coeff.end(); ++itr) {
      while (ans_itr->first > itr->first) {
        ++ans_itr;
      }
      // Add the coefficients in rhs to ans if the term exist.
      // Otherwise, insert it into ans.
      if (itr->first == ans_itr->first) {
        ans_itr->second = ans_itr->second + itr->second;
      }
      else {
        ans.coeff.insert(*itr);
      }
    }
    return ans;
  }

  // Return the negation of this Polynomial.
  // For example, if this Polynomial is 4 * x ^ 3 - 2, return -4 * x ^ 3 + 2
  Polynomial operator-() const {
    Polynomial<Num> ans(*this);
    typename map<int, Num>::iterator itr;
    for (itr = ans.coeff.begin(); itr != ans.coeff.end(); ++itr) {
      itr->second = -(itr->second);
    }
    return ans;
  }

  // Subtract rhs from this Polynomial and return the result
  Polynomial operator-(const Polynomial & rhs) const {
    Polynomial<Num> ans(*this + (-rhs));
    typename map<int, Num>::iterator itr;
    vector<int> toBeRemove;
    for (itr = ans.coeff.begin(); itr != ans.coeff.end(); ++itr) {
      if (itr->second == Num() && itr->first != 0) {
        toBeRemove.push_back(itr->first);
      }
    }
    for (int i = 0; i < toBeRemove.size(); ++i) {
      ans.coeff.erase(toBeRemove[i]);
    }
  }

  // Multiply this Polynomial by a scalar and return the result
  Polynomial operator*(const Num & n) const {
    Polynomial<Num> ans(*this);
    typename map<int, Num>::iterator itr;
    for (itr = ans.coeff.begin(); itr != ans.coeff.end(); ++itr) {
      itr->second = n * itr->second;
    }
    return ans;
  }

  // Multiply this Polynomial by rhs, and return the result.
  Polynomial operator*(const Polynomial & rhs) const {
    Polynomial<Num> ans;
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
    typename map<int, Num>::const_iterator itr;
    for (itr = coeff.begin(); itr != coeff.end(); ++itr) {
      if (rhs.coeff.find(itr->first) == rhs.coeff.end()) {
        return false;
      }
      // Already checked that itr->first is a key in rhs.coeff,
      // so the at() function is no exception guaranteed
      if (rhs.coeff.at(itr->first) != itr->second) {
        return false;
      }
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
    typename map<int, Num>::const_iterator itr;
    for (itr = coeff.begin(); itr != coeff.end(); ++itr) {
      Num term = itr->second;

      // Avoid using +=, *= or pow() here since it is not guaranteed
      // that Num will have these operators or functions.
      for (int i = 0; i < itr->first; ++i) {
        term = term * x;
      }
      ans = ans + term;
    }
    return ans;
  }

  // This takes the derivative of this Polynomial, resulting in another Polynomial.
  Polynomial derivative() const {
    // Polynomial always keeps the x^0 term even it has its coefficient==0,
    // so coeff.size() == 1 means it only has x^0 term and its derivative is 0.
    if (coeff.size() == 1) {
      return Polynomial();
    }

    Polynomial<Num> ans;
    typename map<int, Num>::const_iterator itr;
    for (itr = coeff.begin(); itr != coeff.end(); ++itr) {
      if (itr->first > 0) {
        ans.addTerm(itr->second * itr->first, itr->first - 1);
      }
    }
    return ans;
  }

  //Use the Newton-Raphson method of numerically finding the zero of a Polynomial.
  Num findZero(Num x, unsigned maxSteps, double tolerance) {
    Polynomial<Num> fx(*this);
    for (int i = maxSteps; i > 0; --i) {
      //cout << i << " step remaining x= " << x << ", f(x) = " << fx.eval(x)
      //     << ", f'(x) = " << fx.derivative().eval(x) << endl;
      if (abs(fx.eval(x)) <= tolerance) {
        return x;
      }
      if (fx.derivative().eval(x) == Num()) {
        throw convergence_failure<Num>(x);
      }
      x = x - (fx.eval(x) / fx.derivative().eval(x));
    }

    //cout << 0 << " step remaining x= " << x << ", f(x) = " << fx.eval(x)
    //     << ", f'(x) = " << fx.derivative().eval(x) << endl;
    if (abs(fx.eval(x)) > tolerance) {
      throw convergence_failure<Num>(x);
    }
    return x;
  }

  template<typename N>
  friend std::ostream & operator<<(std::ostream & os, const Polynomial<N> & p);
};

// Write the Polynomial to "os" such that it looks like this:
// "4*x^4 + 2*x^3 + -7*x^2 + 0*x^1 + -9*x^0"
template<typename N>
std::ostream & operator<<(std::ostream & os, const Polynomial<N> & p) {
  if (p == Polynomial<N>()) {
    os << N();
    return os;
  }

  typename map<int, N>::const_iterator itr = p.coeff.begin();
  os << itr->second << "*x^" << itr->first;
  itr++;
  while (itr != p.coeff.end()) {
    if (itr->second != N()) {
      os << " + ";
      os << itr->second << "*x^" << itr->first;
    }
    ++itr;
  }
  return os;
}
