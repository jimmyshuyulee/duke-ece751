#include <algorithm>
#include <fstream>
#include <memory>
#include <regex>
#include <sstream>
#include <vector>

#include "graph.hpp"
//#include "logging.hpp"

std::string getArguments(std::stringstream & ss) {
  std::stringstream args;
  std::string temp;
  ss >> temp;
  int n = std::count(temp.begin(), temp.end(), '[');
  n -= std::count(temp.begin(), temp.end(), ']');
  args << temp + " ";
  while (n > 0) {
    ss >> temp;
    n += std::count(temp.begin(), temp.end(), '[');
    n -= std::count(temp.begin(), temp.end(), ']');
    args << temp + " ";
  }

  // std::cout << args.str() << std::endl;
  std::string s = args.str();
  return s;
}

// Only check the type and format of input arguments here. The validity of
// those arguments will be checked in createTask().
void convertTask(std::unique_ptr<Graph> & graph, std::string & str) {
  std::stringstream ss(str);
  std::string task_id, task_name, task_type;
  ss >> task_id;
  if (!std::regex_match(task_id, std::regex("[0-9]*"))) {
    throw std::invalid_argument("Task ID is not digits");
  }
  ss >> task_name;
  ss >> task_type;

  std::string task_arguments = getArguments(ss);
  if (task_arguments.find('[') == std::string::npos ||
      task_arguments.find(']') == std::string::npos) {
    std::cerr << "Missing bracket for arguments." << std::endl;
    exit(EXIT_FAILURE);
  }
  // std::cout << task_arguments.substr(1, task_arguments.size() - 2) <<
  // std::endl;

  std::shared_ptr<Task> t = createTask(
      task_type,
      stoi(task_id),
      task_name,
      task_arguments.substr(task_arguments.find('[') + 1, task_arguments.rfind(']') - 1));
  graph->addTask(t);
}

void convertDependency(std::unique_ptr<Graph> & graph, std::string & str) {
  // Check the validity of the expression here
  std::string nodes = "[0-9]*([[:space:]]*,[[:space:]]*[0-9]*)*";
  if (!std::regex_match(
          str,
          std::regex(nodes + "[[:space:]]*(>>[[:space:]]*" + nodes + "[[:space:]]*)*"))) {
    throw std::invalid_argument("Incorrect dependency definition.");
  }
  std::vector<std::string> tasks = splitString(str, ">>");
  for (unsigned i = 1; i < tasks.size(); i++) {
    // if tasks[i-1] and tasks[i] are two sets of tasks, create dependency
    // graph based on their cartesian.
    for (auto t1 : splitString(tasks[i - 1], ",")) {
      for (auto t2 : splitString(tasks[i], ",")) {
        graph->addDependency(stoi(t1), stoi(t2));
      }
    }
  }
}

std::unique_ptr<Graph> createGraph(const std::string & str, Logger * logger) {
  std::vector<std::string> cronInfo = splitString(str, " ");
  // Checking cron line format is too complicated, so I only make sure the
  // number of arguments is correct for now.
  if (cronInfo.size() != 6) {
    throw std::invalid_argument("Incorrect execution time setting.");
  }
  std::string name = cronInfo[0];
  std::stringstream time;
  for (unsigned i = 1; i < cronInfo.size(); i++) {
    time << cronInfo[i] << " ";
  }
  std::unique_ptr<Graph> graph(new Graph(name, time.str(), logger));
  return graph;
}

std::unique_ptr<Graph> readWorkflowFile(std::ifstream & ifs, Logger * logger) {
  std::string str;

  bool workflow_definition = false;
  std::getline(ifs, str);
  str.erase(0, str.find_first_not_of(" "));
  str.erase(str.find_last_not_of(" ") + 1);
  std::unique_ptr<Graph> graph(createGraph(str, logger));
  std::getline(ifs, str);
  str.erase(str.find_last_not_of(" ") + 1);
  if (!str.empty()) {
    throw std::invalid_argument(
        "Require one empty line between workflow and tasks definition");
  }
  while (!ifs.eof()) {
    std::getline(ifs, str);
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
    if (str.empty()) {
      workflow_definition = true;
      continue;
    }
    if (workflow_definition) {
      convertDependency(graph, str);
    }
    else {
      convertTask(graph, str);
    }
  }

  // graph->printGraph();

  if (!graph->checkAcyclic()) {
    throw std::invalid_argument("Constructed graph conatains cyclic dependency");
  }
  return graph;
}
