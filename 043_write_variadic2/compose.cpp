#include <iostream>

// TODO: Base case
template<typename Ftype>
Ftype compose(const Ftype & fn) {
  return fn;
}

// TODO: Recursive case
template<typename Ftype, typename... Other>
auto compose(Ftype fn, Other... other) {
  return [fn, other...](auto x) { return compose(other...)(fn(x)); };
}

int main(void) {
  auto add_func = [](const int x) { return x + 5; };
  auto sub_func = [](const int x) { return x - 3; };
  int res = compose(add_func, sub_func)(0);
  std::cout << "Result: " << res << "\n";
}
