#include <thread>

#include "barrier.hpp"
#include "rdtscl.hpp"

typedef unsigned long long cycle_t;
typedef std::pair<cycle_t, cycle_t> enter_exit;
typedef std::pair<bool, int> res_id;
typedef std::pair<res_id, enter_exit> res_id_enter_exit;

/*
 * Wrapper for the enter function.
 * DO NOT EDIT.
 */
bool wrapper(Barrier * b, res_id_enter_exit * r) {
  int id = std::hash<std::thread::id>()(std::this_thread::get_id());
  cycle_t start = rdtscl();
  bool res = b->enter();
  cycle_t end = rdtscl();
  r->first = res_id(res, id);
  r->second = enter_exit(start, end);
  return res;
}

/*
 * A simple test
 */
void test1(void) {
  Barrier * b = new Barrier(3);
  res_id_enter_exit r1, r2, r3;

  std::thread t1(wrapper, b, &r1);
  std::thread t2(wrapper, b, &r2);
  std::thread t3(wrapper, b, &r3);

  t1.join();
  t2.join();
  t3.join();
}

/*
 * TODO Write more tests, add them to main.
 */
int main(void) {
  test1();
  return EXIT_SUCCESS;
}
