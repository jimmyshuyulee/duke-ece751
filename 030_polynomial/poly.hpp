
template<typename Num>
class Polynomial {
  //Default construct the polynomial to be "0" (i.e., 0*x^0)
  Polynomial() {}
  o Polynomial operator+(const Polynomial & rhs) const;
  Add this Polynomial to rhs,
      and return the resulting polynomial.For example if this is 2 * x ^ 2 + 3,
      and rhs is x ^ 2 + 4 * x,
      you should return the Polynomial 3 * x ^
          2 + 4 * x + 3. o Polynomial operator-() const;
  Return the negation of this Polynomial.For example, if this Polynomial is 4 * x ^ 3 - 2,
      return -4 * x ^ 3 + 2 o Polynomial operator-(const Polynomial &rhs) const;
  Subtract rhs from this Polynomial and return the result.o Polynomial operator*(
      const Num & n) const;
  Multiply this Polynomial by a scalar and return the result o Polynomial operator*(
      const Polynomial & rhs) const;
};
