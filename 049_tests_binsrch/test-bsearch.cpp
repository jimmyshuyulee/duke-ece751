#include <cmath>
#include <iostream>

#include "function.h"

extern int binarySearchForZero(Function<int, int> * f, int low, int high);

class CountedIntFn : public Function<int, int> {
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const char * mesg;

 public:
  CountedIntFn(unsigned n, Function<int, int> * fn, const char * m) :
      remaining(n),
      f(fn),
      mesg(m) {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr, "Too many function invocations in %s\n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
};

class LinearFunction : public Function<int, int> {
 public:
  LinearFunction() {}
  virtual int invoke(int arg) { return arg; }
};

class SinFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 10000000 * (sin(arg / 100000.0) - 0.5); }
};

void check(Function<int, int> * f,
           int low,
           int high,
           int expected_ans,
           const char * mesg) {
  CountedIntFn test(log2(high - low) + 1, f, mesg);
  int ans = binarySearchForZero(&test, low, high);
  if (ans != expected_ans) {
    fprintf(stderr, "Expected answer is %d, but get %d\n", expected_ans, ans);
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  SinFunction s;
  LinearFunction f;
  check(&s, 0, 150000, 52359, "sin 0 ~ 150000, 52359");
  check(&f, 0, 2, 0, "0 ~ 2");
  check(&f, -1, 3, 0, "-1 ~ 3");
  check(&f, 100, 2000, 100, "100 ~ 2000");
  check(&f, -10000, -1000, -1001, "-10000 ~ -1000");
  return EXIT_SUCCESS;
}
