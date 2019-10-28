#include <stdlib.h>

#include <iostream>
#include <numeric>
#include <vector>

/*
 * TODO: Implement quicksort   
 */
template<typename T>
void quicksort(std::vector<T> & vec, int L, int R) {
  if (R - L < 1) {
    return;
  }
  int pivot = L;
  for (int i = L; i <= R; i++) {
    if (vec[i] < vec[pivot]) {
      std::swap(vec[i], vec[pivot]);
      std::swap(vec[i], vec[++pivot]);
    }
  }
  quicksort(vec, L, pivot - 1);
  quicksort(vec, pivot + 1, R);
}

/*
 * TODO: Verify the given vector is in order and elements sum
 * to the correct sum
 */
template<typename T>
bool checkSorting(std::vector<T> & vec, T sum) {
  T arraySum(vec[0]);
  for (int i = 1; i < vec.size(); i++) {
    if (vec[i] < vec[i - 1]) {
      return false;
    }
    arraySum = arraySum + vec[i];
  }

  return (arraySum == sum) ? true : false;
}

// tests a large vector of random ints
int test1(int count) {
  std::vector<int> random = std::vector<int>();
  for (int i = 0; i < count; i++) {
    random.push_back(1 + rand() % (count * 50));
  }
  int sum = std::accumulate(random.begin(), random.end(), 0);
  quicksort(random, 0, random.size() - 1);
  return checkSorting(random, sum);
}

// tests a small vector in reverse order
int test2() {
  std::vector<int> reverse({6, 5, 4, 3, 2, 1, 0, -1});
  int sum = std::accumulate(reverse.begin(), reverse.end(), 0);
  quicksort(reverse, 0, reverse.size() - 1);
  return checkSorting(reverse, sum);
}

// tests a small vector of elements that are all equal
int test3() {
  std::vector<int> same({5, 5, 5, 5, 5});
  int sum = std::accumulate(same.begin(), same.end(), 0);
  quicksort(same, 0, same.size() - 1);
  return checkSorting(same, sum);
}

int main(int argc, char ** argv) {
  int count = 100;
  if (argc > 1) {
    count = atoi(argv[1]);
  }
  if (test1(count) && test2() && test3()) {
    std::cout << "Passed!"
              << "\n";
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
