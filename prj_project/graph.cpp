#include "graph.hpp"

std::shared_ptr<Task> Graph::getTask(const TaskID & id) {
  if (tasks_.find(id) == tasks_.end()) {
    std::stringstream msg;
    msg << "Task id " << id << " does not exist.";
    throw std::invalid_argument(msg.str());
  }
  return tasks_[id];
}

std::shared_ptr<Task> Graph::createTask(const std::string & type,
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

void Graph::addTask  //(std::shared_ptr<Task> t) {
    (const std::string & type,
     const TaskID & id,
     const std::string & name,
     const std::string & args) {
  //TaskID id = t->get_id();
  if (tasks_.find(id) != tasks_.end() || adj_list_.find(id) != adj_list_.end()) {
    std::stringstream ss;
    ss << "Multiple assingments on Task id " << id
       << ". Only the first definition is used.";
    std::cerr << ss.str() << std::endl;
    logger_->warning(name_, ss.str());
    return;
  }
  std::shared_ptr<Task> t = createTask(type, id, name, args);
  tasks_[id] = t;
  adj_list_[id] = std::unordered_set<TaskID>();
  upstream_status_[id] = true;
}

void Graph::addDependency(const TaskID & s, const TaskID & t) {
  if (tasks_.find(s) == tasks_.end() || tasks_.find(t) == tasks_.end()) {
    std::stringstream msg;
    msg << "Task id " << (tasks_.find(s) == tasks_.end() ? s : t)
        << " specified in dependency map does not exist.";
    throw std::invalid_argument(msg.str());
  }
  adj_list_[t].insert(s);
}

bool Graph::checkAcyclic() {
  std::vector<TaskID> sorted;
  AdjList graph_to_be_sorted(adj_list_);
  std::queue<TaskID> q;
  for (auto adj : graph_to_be_sorted) {
    if (adj.second.empty()) {
      q.push(adj.first);
    }
  }
  while (!q.empty()) {
    TaskID n = q.front();
    q.pop();
    sorted.push_back(n);
    graph_to_be_sorted.erase(n);
    for (auto itr = graph_to_be_sorted.begin(); itr != graph_to_be_sorted.end(); itr++) {
      if (itr->second.find(n) != itr->second.end()) {
        itr->second.erase(n);
        if (itr->second.empty()) {
          q.push(itr->first);
        }
      }
    }
  }
  return (graph_to_be_sorted.empty()) ? true : false;
}

bool Graph::executeOneTask(const TaskID & a_task_id) {
  bool execute_failure;
  std::stringstream ss;
  if (task_com_->getSkipped(a_task_id)) {
    execute_failure = false;
    ss << "Task " << a_task_id << " is skipped by branching";
    logger_->warning(name_, ss.str());
  }
  else if (!upstream_status_[a_task_id]) {
    execute_failure = true;
    ss << "Task " << a_task_id
       << " is not executed due to failure in its depending tasks";
    logger_->error(name_, ss.str());
  }
  else {
    exitStatusOutputPair es_output = tasks_[a_task_id]->executeTask(task_com_);
    execute_failure = es_output.first;

    if (execute_failure) {
      ss << "Task " << a_task_id << " failed";
      logger_->error(name_, ss.str());
    }
    else {
      ss << "Task " << a_task_id << " is successfully executed";
      logger_->info(name_, ss.str());
    }
    logger_->redirect(es_output.second);
  }
  return execute_failure;
}

void Graph::executionSummary(std::vector<TaskID> failed_tasks) {
  if (failed_tasks.empty()) {
    logger_->info(name_, "All tasks are successfully executed !");
  }
  else {
    std::stringstream ss;
    ss << "The follwing tasks encountered errors during execution: [ ";
    for (auto id : failed_tasks) {
      ss << id << " ";
    }
    ss << "]. Please check their error message for further reference.";
    logger_->warning(name_, ss.str());
  }
}

void Graph::runWorkflow(bool real_execution) {
  AdjList incompleted_tasks(adj_list_);
  std::vector<TaskID> failed_tasks;
  while (!incompleted_tasks.empty()) {
    std::vector<TaskID> available_tasks;
    for (auto i_task : incompleted_tasks) {
      if (i_task.second.empty()) {
        available_tasks.push_back(i_task.first);
      }
    }
    // parallelism can be added
    for (auto a_task_id : available_tasks) {
      incompleted_tasks.erase(a_task_id);
      bool execute_failure;
      // During initial checking step, it only check whether the input and
      // workflow logic are valid, so it will not fail a task on its
      // depending tasks' failure.
      execute_failure = tasks_[a_task_id]->checkTask(task_com_);
      if (real_execution) {
        // During real execution, make sure that every failed tasks, no matter
        // they fail thenselves or due to failure of their depending tasks,
        // have their failing messege recorded in log file.
        execute_failure = executeOneTask(a_task_id);
      }
      if (execute_failure) {
        failed_tasks.push_back(a_task_id);
      }

      for (auto itr = incompleted_tasks.begin(); itr != incompleted_tasks.end(); itr++) {
        if (itr->second.find(a_task_id) != itr->second.end()) {
          itr->second.erase(a_task_id);
          if (execute_failure) {
            upstream_status_[itr->first] = false;
          }
        }
      }
    }
  }
  // Log the execution summary.
  if (real_execution) {
    executionSummary(failed_tasks);
  }
}

void Graph::printGraph() {
  for (auto v : adj_list_) {
    std::cout << v.first << ": ";
    for (auto s : v.second) {
      std::cout << s << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

