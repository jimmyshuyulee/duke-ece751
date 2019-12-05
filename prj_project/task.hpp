#ifndef TASK_H
#define TASK_H
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <regex>
#include <utility>
#include <vector>

typedef unsigned TaskID;
typedef std::unordered_map<TaskID, std::unordered_set<TaskID> > AdjList;
typedef std::pair<int, std::string> exitStatusOutputPair;

std::vector<std::string> splitString(std::string str, const std::string & delimiter);
exitStatusOutputPair executeAndGetOutput(std::string);

class TaskCom {
  const AdjList & adj_list_;
  std::unordered_set<TaskID> skipped_;

 public:
  TaskCom() = default;
  explicit TaskCom(const AdjList & adj_list) :
      adj_list_(adj_list),
      skipped_(std::unordered_set<TaskID>()) {}
  AdjList get_adj_list() noexcept { return adj_list_; }
  bool getSkipped(const TaskID & id) noexcept {
    return (skipped_.find(id) != skipped_.end());
  }
  void setSkipped(const TaskID & id) { skipped_.insert(id); }
  // Return whether curr_id depends on dep_id. Since adj_list_ is a const& here
  // , the [] operator cannot be used.
  bool getDependency(const TaskID & curr_id, const TaskID & dep_id) {
    return (adj_list_.at(curr_id).find(dep_id) != adj_list_.at(curr_id).end());
  }
};

// A base class for all kinds of tasks. It should not be instantiated.
class Task {
 protected:
  TaskID id_;
  std::string name_;
  std::string args_;

  Task() = default;
  Task(const TaskID & id, const std::string & name, const std::string & args) :
      id_(id),
      name_(name),
      args_(args) {}

 public:
  unsigned get_id() const noexcept { return id_; }
  std::string get_name() const noexcept { return name_; }
  virtual int checkTask(std::unique_ptr<TaskCom> & task_com) = 0;
  virtual exitStatusOutputPair executeTask(std::unique_ptr<TaskCom> & task_com) = 0;
  virtual ~Task() {}
};

class BashTask : public Task {
 public:
  BashTask() = default;
  BashTask(const TaskID & id, const std::string & name, const std::string & args) :
      Task(id, name, args) {}
  virtual int checkTask(std::unique_ptr<TaskCom> & task_com) final override { return 0; };
  virtual exitStatusOutputPair executeTask(
      std::unique_ptr<TaskCom> & task_com) final override {
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
  virtual ~BashTask() {}
};

class BranchTask : public Task {
  std::string command_;
  std::unordered_map<std::string, TaskID> output_id_map_;

 public:
  BranchTask() = default;
  BranchTask(const TaskID & id, const std::string & name, const std::string & args) :
      Task(id, name, args),
      command_(std::string()),
      output_id_map_(std::unordered_map<std::string, TaskID>()) {}

  virtual int checkTask(std::unique_ptr<TaskCom> & task_com) final override {
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
      else if (!task_com->getDependency(stoi(branch[1]), id_)) {
        throw std::invalid_argument(
            "Branching destination is not a dependent of the branch task.");
      }
      branch[0].erase(0, branch[0].find_first_not_of(" "));
      branch[0].erase(branch[0].find_last_not_of(" ") + 1);
      output_id_map_[branch[0]] = stoi(branch[1]);
    }
    return 0;
  };

  virtual exitStatusOutputPair executeTask(
      std::unique_ptr<TaskCom> & task_com) final override {
    exitStatusOutputPair es_output = executeAndGetOutput(command_ + " 2>&1");
    if (output_id_map_.find(es_output.second) == output_id_map_.end()) {
      if (es_output.first != 0) {
        std::cout << "Exit status: " << es_output.first << std::endl;
        throw std::runtime_error(es_output.second);
      }
      else {
        throw std::runtime_error(
            "Cannot find any corresponding task for the given output");
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
  virtual ~BranchTask() {}
};

class DummyTask : public Task {
 public:
  DummyTask() = default;
  DummyTask(const TaskID & id, const std::string & name, const std::string & args) :
      Task(id, name, args) {}
  virtual int checkTask(std::unique_ptr<TaskCom> & task_com) final override { return 0; };
  virtual exitStatusOutputPair executeTask(
      std::unique_ptr<TaskCom> & task_com) final override {
    return std::make_pair(0, std::string());
  }
  virtual ~DummyTask() {}
};

std::shared_ptr<Task> createTask(const std::string & type,
                                 const TaskID & id,
                                 const std::string & name,
                                 const std::string & args) {
  if (type == "bash") {
    return std::shared_ptr<Task>(new BashTask(id, name, args));
  }
  else if (type == "branch") {
    return std::shared_ptr<Task>(new BranchTask(id, name, args));
  }
  else if (type == "dummy") {
    return std::shared_ptr<Task>(new DummyTask(id, name, args));
  }
  else {
    return nullptr;  // new Task(id, name, args);
  }
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

exitStatusOutputPair executeAndGetOutput(std::string cmd) {
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
#endif
