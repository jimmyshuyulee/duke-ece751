#ifndef GRAPH_H
#define GRAPH_H
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "logging.hpp"
#include "task.hpp"

class Graph {
  std::string name_;
  // This variable stores the scheduled time for this workflow to be executed
  std::string time_;
  std::unordered_map<TaskID, std::shared_ptr<Task> > tasks_;
  AdjList adj_list_;

  // This variable records whether each task's upstream tasks (its dependencies)
  // are all executed successfully.
  std::unordered_map<TaskID, bool> upstream_status_;

  // This object is for Graph to communicate with its tasks. This implementation
  // is to allow all tasks to share some information and prevent its tasks from
  // accessing some of Graph's member.
  std::unique_ptr<TaskCom> task_com_;

  // I plan to implement multithreading for parrallel tasks in the future. In
  // order to allow multiple threads to write to the same logger, I currently
  // keep it as a normal pointer.
  Logger * logger_;

  // Create the corresponding subclass of Task based on inputted type.
  std::shared_ptr<Task> createTask(const std::string & type,
                                   const TaskID & id,
                                   const std::string & name,
                                   const std::string & args);

  // Execute the inputted task. Called in runWorkflow().
  bool executeOneTask(const TaskID & a_task_id);

  // Summerize execution result. It will specify all of the failed tasks' id
  // if there is any of them.
  void executionSummary(std::vector<TaskID> failed_tasks);

  // Abstracted function for workflow execution. Used in checkWorkflow() and
  // executeWorkflow().
  void runWorkflow(bool real_execution);

 public:
  Graph() : Graph("", "", nullptr) {}
  Graph(const std::string & name, const std::string & time, Logger * logger) :
      name_(name),
      time_(time),
      tasks_(std::unordered_map<TaskID, std::shared_ptr<Task> >()),
      adj_list_(AdjList()),
      upstream_status_(std::unordered_map<TaskID, bool>()),
      task_com_(new TaskCom(adj_list_)),
      logger_(logger) {}

  // At this stage delete copy constructor and copy assignment operator.
  // The graph can only be constructed from a workflow definition file
  Graph(const Graph & rhs) = delete;
  Graph & operator=(const Graph & rhs) = delete;
  inline std::string get_time() { return time_; }
  inline std::shared_ptr<Task> getTask(const TaskID & id);

  // It will call createTask() to instantiate one kind of Task object.
  void addTask(const std::string & type,
               const TaskID & id,
               const std::string & name,
               const std::string & args);

  // This funtion adds an edge from s to t, which means the execution of s
  // depends on the result of t
  void addDependency(const TaskID & s, const TaskID & t);

  // Use topological sort to check whether this graph contains a cycle
  bool checkAcyclic();

  // This function checks the logic of the workflow, for example whether branch
  // tasks branch the flow to its following tasks
  void checkWorkflow() { runWorkflow(false); }

  // This function actually runs the workflow
  void executeWorkflow() {
    logger_->info(name_, "Start running " + name_);
    runWorkflow(true);
  }

  // Print the adj list. Currently just for debugging
  void printGraph();
  ~Graph() {}
};
#endif
