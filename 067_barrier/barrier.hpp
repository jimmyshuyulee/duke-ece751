#include <condition_variable>
#include <mutex>

using std::condition_variable;
using std::mutex;

class Barrier {
 private:
  // TODO: Fill in fields
  mutex mtx;
  condition_variable cv;
  int busy_thread;

 public:
  // TODO: Constructor
  Barrier(int n) : busy_thread(n) {}
  bool enter();
};

