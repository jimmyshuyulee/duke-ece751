#include "barrier.hpp"

#include <iostream>
#include <thread>

/*
 * TODO: Implement
 */
bool Barrier::enter() {
  busy_thread--;
  while (busy_thread > 1) {
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck);
  }
  cv.notify_all();
  return true;
}
