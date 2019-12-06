#ifndef LOGGING_H
#define LOGGING_H
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std::chrono;
class Logger {
  std::ostream * os_;

 public:
  explicit Logger(std::ostream & os = std::cout) : os_(&os) {}

  void createMessage(const std::string & task_name,
                     const std::string & msg,
                     const std::string & level) {
    time_t tt = system_clock::to_time_t(system_clock::now());
    auto lt = localtime(&tt);

    std::stringstream ss;
    // Add timestamp
    ss << "[";
    ss << lt->tm_year + 1900 << "-" << lt->tm_mon << "-" << lt->tm_mday << " "
       << lt->tm_hour << ":" << lt->tm_min << ":" << lt->tm_sec;
    ss << " - " << task_name;
    // Add severity level of the message
    ss << " - " << std::left << std::setw(7) << level;
    ss << "] ";
    ss << msg << std::endl;
    *os_ << ss.rdbuf();
  }
  void fatal(const std::string & task_name, const std::string & msg) {
    createMessage(task_name, msg, "FATAL");
  }
  void error(const std::string & task_name, const std::string & msg) {
    createMessage(task_name, msg, "ERROR");
  }
  void warning(const std::string & task_name, const std::string & msg) {
    createMessage(task_name, msg, "WARNING");
  }
  void info(const std::string & task_name, const std::string & msg) {
    createMessage(task_name, msg, "INFO");
  }
  void debug(const std::string & task_name, const std::string & msg) {
    createMessage(task_name, msg, "DEBUG");
  }
  // This function is specially designed for redirecting stdout and stderr of
  // executed tasks to out lo file. SInce those outputs are not directed
  // related to this program, I decided not to add logging tags before them.
  void redirect(const std::string & msg) {
    if (!msg.empty()) {
      std::stringstream ss(msg + "\n");
      *os_ << ss.rdbuf();
    }
  }
};
#endif
