#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <array>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <thread>

// namespace so that names don't clash
namespace ECE751 {

  template<unsigned nThreads = 10>
  class ThreadPool {
    // TODO: Define variables and data structures needed
    typedef std::list<std::function<void(void)> > task_queue_t;
    std::mutex mtx;
    std::condition_variable cv;
    std::mutex end_mtx;
    std::condition_variable end_cv;
    task_queue_t tasks;
    std::list<std::thread> workers;
    std::atomic<bool> join_all;
    std::atomic<unsigned> remaining_tasks;

    /* TODO:
     * Pulls a task off the queue and runs it
     * Signals to main when it is done
     */
    void Task() {
      while (!join_all) {
        std::function<void(void)> task(get_next_task());
        task();
        remaining_tasks--;
        if (remaining_tasks == 0) {
          end_cv.notify_one();
        }
      }
      return;
    }

    /* TODO:
     * Signals to main to indicate that we are executing
     * a task and work may be done.
     * Then grabs a task off of the task queue and executes
     * it, otherwise it will wait indefinitely for a task
     */
    std::function<void(void)> get_next_task() {
      std::function<void(void)> res;
      std::unique_lock<std::mutex> lck(mtx);
      while (tasksRemaining() < 1) {
        cv.wait(lck);
      }
      res = tasks.front();
      tasks.pop_front();
      return res;
    }

   public:
    /* TODO:
     * Implement constructor
     */
    ThreadPool() : tasks(task_queue_t()), join_all(false), remaining_tasks(0) {
      for (unsigned i = 0; i < nThreads; i++) {
        workers.emplace_back([this] { this->Task(); });
      }
    }

    /* TODO:
     * Destructor
     */
    ~ThreadPool() { joinThreads(); }

    /* TODO:
     * Returns number of threads
     */
    inline unsigned size() const { return nThreads; }

    /* TODO:
     * Returns number of remaining tasks
     */
    inline unsigned tasksRemaining() { return tasks.size(); }

    /* TODO:
     * This function enqueues a particular tasks and waits
     * for it to run to completion.
     */
    void runTaskToCompletion(std::function<void(void)> task) {
      enqueue(task);
      waitAll();
    }

    /* TODO:
     * Enqueues a new task onto the pool. A thread will be woken
     * up to execute the task if one is available.
     */
    void enqueue(std::function<void(void)> task) {
      {
        std::unique_lock<std::mutex> lck(mtx);
        tasks.push_back(task);
        remaining_tasks++;
      }

      cv.notify_one();
    }

    /* TODO:
     * Join all threads - will by default wait until completion,
     * otherwise work will be incomplete
     * Use only if you are done with the threadpool
     */
    void joinThreads(bool waitForAll = true) {
      if (waitForAll) {
        waitAll();
      }
      {
        std::unique_lock<std::mutex> lck(mtx);
        join_all = true;
        for (unsigned i = 0; i < nThreads; i++) {
          tasks.push_back([] { return; });
          remaining_tasks++;
        }
      }
      cv.notify_all();
      for (std::list<std::thread>::iterator itr = workers.begin(); itr != workers.end();
           itr++) {
        itr->join();
      }
    }

    /* TODO:
     * Wait until there are no remaining tasks
     */
    void waitAll() {
      //std::unique_lock<std::mutex> add_lck(add_mtx);
      while (tasksRemaining() > 0 || remaining_tasks > 0) {
        std::unique_lock<std::mutex> lck(end_mtx);
        end_cv.wait(lck);
      }
    }
  };

}  // namespace ECE751

#endif
