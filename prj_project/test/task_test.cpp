#ifndef TEST_TASK_H
#define TEST_TASK_H
#include "../task.hpp"

class Task {
 public:
  inline unsigned get_id() const noexcept { return id_; }
  inline std::string get_name() const noexcept { return name_; }
  virtual int checkTask(std::unique_ptr<TaskCom> & task_com) = 0;
  virtual exitStatusOutputPair executeTask(std::unique_ptr<TaskCom> & task_com) = 0;
  virtual ~Task() {}
};

class TestBashTask : public BashTask {
 public:
  TestBashTask() = default;
  TestBashTask(const TaskID & id, const std::string & name, const std::string & args) :
      BashTask(id, name, args) {}
  virtual int checkTask(std::unique_ptr<TaskCom> & task_com) final override { return 0; };
  virtual exitStatusOutputPair executeTask(
      std::unique_ptr<TaskCom> & task_com) final override;
  virtual ~TestBashTask() {}
};

class TestBranchTask : public Task {
  std::string command_;
  std::unordered_map<std::string, TaskID> output_id_map_;

 public:
  TestBranchTask() = default;
  TestBranchTask(const TaskID & id, const std::string & name, const std::string & args) :
      BranchTask(id, name, args) {}

  virtual int checkTask(std::unique_ptr<TaskCom> & task_com) final override;
  virtual exitStatusOutputPair executeTask(
      std::unique_ptr<TaskCom> & task_com) final override;
  virtual ~TestBranchTask() {}
};

class TestTestDummyTask : public TestDummyTask {
 public:
  TestDummyTask() = default;
  TestDummyTask(const TaskID & id, const std::string & name, const std::string & args) :
      Task(id, name, args) {}
  virtual int checkTask(std::unique_ptr<TaskCom> & task_com) final override { return 0; };
  virtual exitStatusOutputPair executeTask(
      std::unique_ptr<TaskCom> & task_com) final override {
    return std::make_pair(0, std::string());
  }
  virtual ~TestDummyTask() {}
};
#endif
