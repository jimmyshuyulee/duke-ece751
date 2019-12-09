
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
  std::string str;
  std::getline(f, str);

  std::stringstream ss(str);
  std::string s1;
  std::string s2;
  ss >> s1;
  ss >> s2;
  return std::make_pair(stoi(s1), stoi(s2));
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
  std::string line;
  unsigned int dev = 0;
  while (getline(f, line)) {
    std::istringstream is(line);
    for (unsigned int task = 0; task < matrixSize.second; ++task) {
      is >> res[dev][task];
    }
    ++dev;
  }
  return res;
  /* for (unsigned i = 0; i < res.size(); i++) {
    std::string str;
    std::getline(f, str);
    std::size_t pos1 = 0;
    int j = 0;
    while (pos1 != std::string::npos) {
      if (pos1 != 0) {
        pos1 += 1;
      }
      std::size_t pos2 = str.find(' ', pos1 + 1);
      res[i][j++] = stod(str.substr(pos1, pos2 - pos1));
      pos1 = pos2;
    }
  }
  return res;
  */
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
  size_t past_tasks = history[0].size();
  std::vector<double> schedule_per_dev(estimates.size(), 0);
  for (unsigned int idx = 0; idx < task_mapping.size(); ++idx) {
    unsigned int dev = task_mapping[idx];
    unsigned int task_idx = rand() % past_tasks;
    schedule_per_dev[dev] += estimates[dev][idx] / history[dev][task_idx];
  }
  return *std::max_element(schedule_per_dev.begin(), schedule_per_dev.end());
  /*
  double ans = 0;
  for (unsigned i = 0; i < task_mapping.size(); i++) {
    double rand_num = rand() % history[i].size();
    ans += estimates[task_mapping[i]][i] * history[i][rand_num];
  }
  return ans;
  */
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
  std::vector<double> trial_res(n_trials);
  for (unsigned int i = 0; i < n_trials; ++i) {
    trial_res[i] = ebs_one_trial(task_mapping, estimates, history);
  }
  std::sort(trial_res.begin(), trial_res.end());
  return trial_res[n_trials * confidence];
  /*
      std::vector<double> record;
  for (unsigned i = 0; i < n_trials; i++) {
    record.push_back(ebs_one_trial(task_mapping, estimates, history));
  }
  std::sort(record.begin(), record.end());
  return record[(int)(record.size() * 0.95)];
*/
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
  if (argc < 3) {
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
