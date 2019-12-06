#include <unistd.h>

#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "graph.hpp"
#include "logging.hpp"
#include "parser.hpp"

enum class Action { Run, Check, Schedule };

// Run, check and schedule all takes the same input arguments. This function
// is an abstraction of their shared part of definition which open file
// streams, call a parser to parse the input and initialize the graph.
void execute(const int & argc, char ** argv, Action action) {
  if (argc != 4) {
    std::cerr << "Invalid argument Number."
              << " Usage: ./workflow action input.txt log_file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream ifs;
  ifs.open(argv[2]);
  if (!ifs.is_open()) {
    std::cerr << "Input file does not exist!" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::ofstream ofs;
  ofs.open(argv[3]);
  if (!ofs.is_open()) {
    std::cerr << "Input file does not exist!" << std::endl;
    exit(EXIT_FAILURE);
  }

  Logger * logger = new Logger(ofs);

  // This function is the input parser
  std::unique_ptr<Graph> graph = readWorkflowFile(ifs, logger);

  switch (action) {
    case Action::Check:
      graph->checkWorkflow();
      break;
    case Action::Run:
      graph->executeWorkflow();
      break;
    case Action::Schedule:
      graph->checkWorkflow();
      char * pwd = get_current_dir_name();
      // Since crontab execute its jobs at home directory, I have to know the
      // absolute file path when scheduling the workflow. Here I assume this
      // program is executed in its own directory.
      std::stringstream ss;
      ss << "(crontab -l ; echo \"" << graph->get_time() << " " << pwd << "/workflow run "
         << pwd << "/" << argv[2] << " " << pwd << "/" << argv[3] << "\") | crontab -";
      std::system(ss.str().c_str());
      free(pwd);
      break;
  }
  ifs.close();
  ofs.close();
  delete logger;
}

// My main can perform run, check and schedule jobs on the specified workflow.
// The action should be the program's first argument.
int main(int argc, char ** argv) {
  if (argc < 2) {
    std::cerr << "Please sepcify your desire action" << std::endl;
    exit(EXIT_FAILURE);
  }

  try {
    if (strcmp(argv[1], "schedule") == 0) {
      execute(argc, argv, Action::Schedule);
    }
    else if (strcmp(argv[1], "run") == 0) {
      execute(argc, argv, Action::Run);
    }
    else if (strcmp(argv[1], "check") == 0) {
      execute(argc, argv, Action::Check);
    }
    else {
      throw std::invalid_argument("Please specify a valid action");
    }
  }
  // I propagate every non-handlable exception to main, to make sure that all
  // the resource are cleaned before exit failure.
  catch (std::exception & e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
