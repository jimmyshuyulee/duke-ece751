#include "studentinterface.hpp"

#include <cfloat>
#include <fstream>
#include <iostream>
#include <string>

Graph * readGraph(std::string fname) {
  Graph * g = new Graph();
  std::ifstream ifs;
  ifs.open(fname);
  std::string str;
  std::vector<unsigned> info;

  std::getline(ifs, str);
  while (!ifs.eof()) {
    // Road id will not be stored here, since I'm not going to use it.
    std::size_t pos1 = str.find(' ');
    while (pos1 != std::string::npos) {
      std::size_t pos2 = str.find(' ', pos1 + 1);
      info.push_back(stoi(str.substr(pos1 + 1, pos2 - pos1 - 1)));
      pos1 = pos2;
    }
    unsigned source = info[0];
    info.erase(info.begin());
    unsigned destination = info[0];
    info.erase(info.begin());
    unsigned length = info[0];
    info.erase(info.begin());
    unsigned maxcars = info[0];
    info.erase(info.begin());

    info.push_back(maxcars);

    g->addEdge(source, destination, length, info);
    info.clear();
    std::getline(ifs, str);
  }
  g->printGraph();
  return nullptr;
}

std::vector<PerCarInfo *> startPlanning(Graph * graph,
                                        const std::vector<start_info_t> & departing_cars);

std::vector<intersection_id_t> getNextStep(
    Graph * graph,
    const std::vector<arrival_info_t> & arriving_cars);

void carArrived(PerCarInfo * finished_cars);

