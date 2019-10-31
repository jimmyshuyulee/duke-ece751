#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP

#include <vector>
#include "threadpool.hpp"

/*
 * TODO:
 * Modify your quicksort from before to make use of your
 * threadpool
 */
template<typename T, typename Compare>
void quicksort(ECE751::ThreadPool<> & pool, std::vector<T> & vec, int L, int R, Compare c) {
}

/*
 * TODO:
 * Execute a parallel quicksort on the given data to completion
 */
template<typename T, typename Compare>
void parallel_qsort(ECE751::ThreadPool<> & pool, std::vector<T> & data, Compare c) {
  pool.runTaskToCompletion([&pool, &data, c] { quicksort(pool, data, 0, data.size() - 1, c); });
}

#endif
