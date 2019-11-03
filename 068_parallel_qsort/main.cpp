#include <assert.h>

#include <iostream>

#include "quicksort.hpp"

int verifyAns(std::vector<int> & vec) {
  if (vec.size() < 2) {
    return 0;
  }
  int prev = vec[0];
  for (unsigned i = 1; i < vec.size(); i++) {
    if (prev > vec[i])
      return -1;
    prev = vec[i];
  }
  return 0;
}

// basic test
int test1(ECE751::ThreadPool<> & pool) {
  int JOB_COUNT = 5;
  auto task = [] {
    std::cout << "Hello "
              << "\n";
  };
  auto task2 = [&pool, task, JOB_COUNT] {
    for (int i = 0; i < JOB_COUNT; i++) {
      pool.enqueue([task]() { task(); });
    };
  };
  pool.runTaskToCompletion(task2);
  return 0;
}

// pqs test
int test2(ECE751::ThreadPool<> & pool) {
  int count = 100;
  auto comp = [](int a, int b) { return a < b; };
  std::vector<int> toSort(count);
  for (int i = 0; i < count; i++) {
    toSort[i] = 1 + rand() % (count * 50);
  }
  parallel_qsort(pool, toSort, comp);
  assert(!verifyAns(toSort));
  return 0;
}

// better pqs test
int test3(ECE751::ThreadPool<> & pool) {
  int count = 100;
  int rounds = 5;
  auto comp = [](int a, int b) { return a < b; };
  std::vector<int> toSort;
  for (int i = 0; i < count; i++) {
    for (int j = 0; j < rounds; j++) {
      toSort.push_back(1 + rand() % (count & 50));
    }
    parallel_qsort(pool, toSort, comp);
    assert(!verifyAns(toSort));
  }
  return 0;
}

int main(int argc, char ** argv) {
  ECE751::ThreadPool<> pool;
  test1(pool);
  test2(pool);
  test3(pool);
  return EXIT_SUCCESS;
}
