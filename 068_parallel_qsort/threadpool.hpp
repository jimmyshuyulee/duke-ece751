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

    /* TODO:
     * Pulls a task off the queue and runs it
     * Signals to main when it is done
     */
    void Task() {
    }

    /* TODO:
     * Signals to main to indicate that we are executing
     * a task and work may be done.
     * Then grabs a task off of the task queue and executes
     * it, otherwise it will wait indefinitely for a task
     */
    std::function<void(void)> get_next_task() {
      std::function<void(void)> res;
      return res;
    }

   public:
    /* TODO:
     * Implement constructor
     */
    ThreadPool() {}

    /* TODO:
     * Destructor
     */
    ~ThreadPool() { }

    /* TODO:
     * Returns number of threads
     */
    inline unsigned size() const { return 0; }

    /* TODO:
     * Returns number of remaining tasks
     */
    inline unsigned tasksRemaining() { return 0; }

    /* TODO:
     * This function enqueues a particular tasks and waits
     * for it to run to completion.
     */
    void runTaskToCompletion(std::function<void(void)> task) {
    }

    /* TODO:
     * Enqueues a new task onto the pool. A thread will be woken
     * up to execute the task if one is available.
     */
    void enqueue(std::function<void(void)> task) {
    }

    /* TODO:
     * Join all threads - will by default wait until completion,
     * otherwise work will be incomplete
     * Use only if you are done with the threadpool
     */
    void joinThreads(bool waitForAll = true) {
    }

    /* TODO:
     * Wait until there are no remaining tasks
     */
    void waitAll() {
    }
  };

}  // namespace ECE751

#endif
