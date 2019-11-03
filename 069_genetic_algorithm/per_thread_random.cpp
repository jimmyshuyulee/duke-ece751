#include <stdlib.h>

#include <thread>

static thread_local unsigned int per_thread_random_seed;
static thread_local int per_thread_random_init = 0;
int per_thread_random() {
  if (per_thread_random_init == 0) {
    std::hash<std::thread::id> hasher;
    per_thread_random_seed = hasher(std::this_thread::get_id());
    per_thread_random_init = 1;
  }
  return rand_r(&per_thread_random_seed);
}
