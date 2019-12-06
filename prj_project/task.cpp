#include "task.hpp"

//virtual int BashTask::checkTask(std::unique_ptr<TaskCom> & task_com) final override {
//  return 0;
//};
exitStatusOutputPair BashTask::executeTask(std::unique_ptr<TaskCom> & task_com) {
  exitStatusOutputPair es_output = executeAndGetOutput(args_ + " 2>&1");

  /*
      std::cout << status << std::endl;
       for (auto i : task_com->get_adj_list()) {
         std::cout << i.first << " : ";
         for (auto j : i.second) std::cout << j << " ";
         std::cout << std::endl;
       }
   */
  return es_output;
}

int BranchTask::checkTask(std::unique_ptr<TaskCom> & task_com) {
  std::vector<std::string> branches = splitString(args_, ",");
  // check len here
  command_ = branches[0];
  for (unsigned i = 1; i < branches.size(); i++) {
    std::vector<std::string> branch = splitString(branches[i], ":");
    if (branch.size() != 2) {
      // check format
    }
    if (!std::regex_match(branch[1], std::regex("[0-9]*"))) {
      throw std::invalid_argument("Branching destination should be a task id.");
    }
    else if (task_com->get_adj_list().find(stoi(branch[1])) ==
             task_com->get_adj_list().end()) {
      throw std::invalid_argument("Branching destination is not in this graph.");
    }
    else if (!task_com->getDependency(stoi(branch[1]), id_)) {
      throw std::invalid_argument(
          "Branching destination is not a dependent of the branch task.");
    }
    branch[0].erase(0, branch[0].find_first_not_of(" "));
    branch[0].erase(branch[0].find_last_not_of(" ") + 1);
    output_id_map_[branch[0]] = stoi(branch[1]);
  }
  return 0;
}

exitStatusOutputPair BranchTask::executeTask(std::unique_ptr<TaskCom> & task_com) {
  exitStatusOutputPair es_output = executeAndGetOutput(command_ + " 2>&1");
  if (output_id_map_.find(es_output.second) == output_id_map_.end()) {
    if (es_output.first != 0) {
      std::cout << "Exit status: " << es_output.first << std::endl;
      throw std::runtime_error(es_output.second);
    }
    else {
      throw std::runtime_error("Cannot find any corresponding task for the given output");
    }
  }
  for (auto p : output_id_map_) {
    if (p.first != es_output.second) {
      task_com->setSkipped(p.second);
    }
  }

  if (es_output.first != 0) {
    //   return es_output;
  }
  return std::make_pair(0, std::string());
}

std::vector<std::string> splitString(std::string str, const std::string & delimiter) {
  std::vector<std::string> splitted_string;
  std::size_t pos = 0;
  while ((pos = str.find(delimiter)) != std::string::npos) {
    splitted_string.push_back(str.substr(0, pos));
    str.erase(0, pos + delimiter.size());
  }
  splitted_string.push_back(str.substr(0, pos));
  return splitted_string;
}

exitStatusOutputPair executeAndGetOutput(const std::string & cmd) {
  std::string result;
  FILE * pipe = popen(cmd.c_str(), "r");
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  char * line = nullptr;
  std::size_t sz = 0;
  ssize_t len = 0;
  while ((len = getline(&line, &sz, pipe)) >= 0) {
    line[len - 1] = '\0';
    result += line;
  }
  free(line);
  int es = pclose(pipe);
  // result.erase(result.find_last_not_of("\n") + 1);
  return std::make_pair(es, result);
}
