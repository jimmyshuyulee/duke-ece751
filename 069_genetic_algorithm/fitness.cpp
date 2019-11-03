#include <limits.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "ga.hpp"
#include "per_thread_random.hpp"
#include "threadpool.hpp"

/*
 * Helper function to read matrix size
 * in the files it is the first line, so given an
 * ifstream we read a single line (should be the first)
 * and return a pair consisting of the nDevs, nTasks
 * where nTasks is either the number of estimates or
 * number of actual
 */
std::pair<size_t, size_t> read_matrix_size(std::ifstream & f) {
  return std::make_pair(0, 0);
}

/*
 * Reads a dev file (located in ../testfiles)
 * the first line is the size, which is read from the
 * above helper, each row is a developer (increasing id
 * starting at 0) and each column represents a task.
 */
std::vector<std::vector<double> > read_dev_file(std::string fname) {
  std::ifstream f(fname);
  std::pair<size_t, size_t> matrixSize = read_matrix_size(f);
  std::vector<std::vector<double> > res(matrixSize.first,
                                        std::vector<double>(matrixSize.second));
  // Read the matrix
  return res;
}

/*
 * Performs one trial of EBS. This involves going through the task 
 * mapping, computing an adjusted estimate based on a randomly selected
 * value from the history and summing it across all tasks per developer.
 * Returns the maximum estimate.
 */
double ebs_one_trial(const std::vector<unsigned> & task_mapping,
                     const std::vector<std::vector<double> > & estimates,
                     const std::vector<std::vector<double> > & history) {
  return 0;
}

/*
 * Performs n_trials of EBS, accumulating the result. Once the result is
 * accumulated, return a result based on the given confidence interval.
 * Note not to use parallel_qsort here..
 */
double evidence_based_scheduling(const std::vector<unsigned> & task_mapping,
                                 const std::vector<std::vector<double> > & estimates,
                                 const std::vector<std::vector<double> > & history,
                                 unsigned n_trials,
                                 double confidence) {
  return 0;
}

/*
 * EBSGene provides the appropriate Gene template expected by
 * GeneticAlgorithm in ga.hpp.
 */
class EBSGene {
  std::shared_ptr<std::vector<std::vector<double> > > estimates;
  std::shared_ptr<std::vector<std::vector<double> > > actual_ratios;
  std::vector<unsigned> my_task_to_dev_mapping;

 public:
  EBSGene() : estimates(nullptr), actual_ratios(nullptr), my_task_to_dev_mapping(0) {}
  EBSGene(std::shared_ptr<std::vector<std::vector<double> > > est,
          std::shared_ptr<std::vector<std::vector<double> > > ratios,
          size_t num_tasks) :
      estimates(est),
      actual_ratios(ratios),
      my_task_to_dev_mapping(num_tasks) {}
  const std::vector<unsigned> & myMapping() const { return my_task_to_dev_mapping; }
  unsigned & operator[](size_t i) { return my_task_to_dev_mapping[i]; }
  const unsigned & operator[](size_t i) const { return my_task_to_dev_mapping[i]; }
  size_t num_tasks() const { return my_task_to_dev_mapping.size(); }
  const std::shared_ptr<std::vector<std::vector<double> > > & getEstimates() const {
    return estimates;
  }
  const std::shared_ptr<std::vector<std::vector<double> > > & getHistory() const {
    return actual_ratios;
  }
};

struct EBSInitParams {
  std::shared_ptr<std::vector<std::vector<double> > > estimates;
  std::shared_ptr<std::vector<std::vector<double> > > actual_ratios;
  size_t num_tasks;
  unsigned num_devs;
};

/*
 * Creates a random gene given the initialization parameters
 */
EBSGene ebs_makeRandomGene(const EBSInitParams & ip) {
  EBSGene g(ip.estimates, ip.actual_ratios, ip.num_tasks);
  for (size_t i = 0; i < ip.num_tasks; i++) {
    g[i] = per_thread_random() % ip.num_devs;
  }
  return g;
}

/*
 * Breeds two genes together.
 */
EBSGene ebs_breed(const EBSGene & g1, const EBSGene & g2) {
  EBSGene ans(g1.getEstimates(), g1.getHistory(), g1.num_tasks());
  for (size_t i = 0; i < g1.num_tasks(); i++) {
    ans[i] = (per_thread_random() & 1) == 0 ? g1[i] : g2[i];
  }
  return ans;
}

/*
 * Fitness function for EBSGene - passes appropriate arguments to
 * evidence_based_scheduling, which you implement above.
 */
double ebs_fitness(const EBSGene & g1) {
  return evidence_based_scheduling(
      g1.myMapping(), *g1.getEstimates(), *g1.getHistory(), 10000, 0.95);
}

/*
 * Helper function that will initialize EBSInitParams
 */
EBSInitParams * initializeParams(std::vector<std::vector<double> > estimates,
                                 std::vector<std::vector<double> > actual) {
  EBSInitParams * i = new EBSInitParams;
  i->estimates = std::make_shared<std::vector<std::vector<double> > >(estimates);
  i->actual_ratios = std::make_shared<std::vector<std::vector<double> > >(actual);
  i->num_devs = estimates.size();
  i->num_tasks = estimates[0].size();
  return i;
}

/*
 * Main illustrates how your code will be used. Note that read_dev_file is
 * used to read both the estimate and history files. It will then initialize
 * ThreadPool and GeneticAlgorithm with appropriate parameters.
 */
int main(int argc, char ** argv) {
  if (argc < 4) {
    std::cout << "Incorrect # of arguments\n";
    return EXIT_FAILURE;
  }
  std::vector<std::vector<double> > estimates = read_dev_file(argv[1]);
  std::vector<std::vector<double> > actual = read_dev_file(argv[2]);
  std::vector<unsigned> task_mapping(estimates[0].size());
  for (size_t i = 0; i < task_mapping.size(); i++) {
    task_mapping[i] = rand() % estimates.size();
  }
  EBSInitParams * i = initializeParams(estimates, actual);
  ECE751::ThreadPool<> pool;
  GeneticAlgorithm<EBSGene, EBSInitParams, ebs_makeRandomGene, ebs_breed, ebs_fitness> ga(
      10, 5, 5);
  EBSGene res = ga.run(pool, *i);
  std::cout << "Resulting gene: \n";
  std::for_each(res.myMapping().begin(), res.myMapping().end(), [](int a) {
    std::cout << a << "\n";
  });
  std::cout << "fitness: " << ebs_fitness(res) << "\n";
  delete i;
  return EXIT_SUCCESS;
}
