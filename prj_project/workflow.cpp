#include <unistd.h>

#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "graph.hpp"
#include "parser.hpp"
//#include "logging.hpp"
enum class Action { Run, Check, Schedule };
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

      std::stringstream ss;
      ss << "(crontab -l ; echo \"" << graph->get_time() << " " << pwd << "/workflow run "
         << pwd << "/" << argv[2] << " " << pwd << "/" << argv[3] << "\") | crontab -";
      //std::cout << ss.str() << "\n";
      std::system(ss.str().c_str());
      free(pwd);
      break;
  }

  ifs.close();
  ofs.close();
  delete logger;
}

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
    else if (strcmp(argv[1], "test") == 0) {
    }
    else {
      throw std::invalid_argument("Please specify a valid action");
    }
  }
  catch (std::exception & e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
