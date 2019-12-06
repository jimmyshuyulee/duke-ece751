#ifndef TASK_H
#define TASK_H
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

typedef unsigned TaskID;
typedef std::unordered_map<TaskID, std::unordered_set<TaskID> > AdjList;
typedef std::pair<int, std::string> exitStatusOutputPair;

// This is a helper function to split a string by a given delimiter.
std::vector<std::string> splitString(std::string str, const std::string & delimiter);

exitStatusOutputPair executeAndGetOutput(const std::string & cmd);

class TaskCom {
  const AdjList & adj_list_;
  std::unordered_set<TaskID> skipped_;

 public:
  TaskCom() = default;
  explicit TaskCom(const AdjList & adj_list) :
      adj_list_(adj_list),
      skipped_(std::unordered_set<TaskID>()) {}
  AdjList get_adj_list() noexcept { return adj_list_; }
  inline bool getSkipped(const TaskID & id) noexcept {
    return (skipped_.find(id) != skipped_.end());
  }
  inline void setSkipped(const TaskID & id) { skipped_.insert(id); }
  // Return whether curr_id depends on dep_id. Since adj_list_ is a const& here
  // , the [] operator cannot be used. The use of this function already make
  // sure that curr_id and dep_id are in the adj_list_.
  inline bool getDependency(const TaskID & curr_id, const TaskID & dep_id) {
    return (adj_list_.at(curr_id).find(dep_id) != adj_list_.at(curr_id).end());
  }
};

// A base class for all kinds of tasks. It should not be instantiated.
class Task {
 protected:
  TaskID id_;
  std::string name_;
  std::string args_;

  //Constructors are protected, so cannot be instaniated.
  Task() = default;
  Task(const TaskID & id, const std::string & name, const std::string & args) :
      id_(id),
      name_(name),
      args_(args) {}
  Task(const Task & rhs) = delete;
  Task & operator=(const Task & rhs) = delete;

 public:
  inline unsigned get_id() const noexcept { return id_; }
  inline std::string get_name() const noexcept { return name_; }
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
      std::unique_ptr<TaskCom> & task_com) final override;
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

  virtual int checkTask(std::unique_ptr<TaskCom> & task_com) final override;
  virtual exitStatusOutputPair executeTask(
      std::unique_ptr<TaskCom> & task_com) final override;
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

/*
std::shared_ptr<Task> createTask(const std::string & type,
                                 const TaskID & id,
                                 const std::string & name,
                                 const std::string & args);
*/
#endif
