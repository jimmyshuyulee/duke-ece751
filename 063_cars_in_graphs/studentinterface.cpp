#include "studentinterface.hpp"

#include <cfloat>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>

using std::vector;

Graph * readGraph(std::string fname) {
  Graph * g = new Graph();
  std::ifstream ifs;
  ifs.open(fname);
  std::string str;
  vector<unsigned> info;

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

vector<intersection_id_t> dijkstra(Graph * graph,
                                   intersection_id_t s,
                                   intersection_id_t d) {
  std::priority_queue<intersection_id_t> pq;
  vector<intersection_id_t> = path;
  pq.push(s);
  vector<> visited(, );
  while (!pq.empty()) {
  }

  return path;
}

vector<PerCarInfo *> startPlanning(Graph * graph,
                                   const std::vector<start_info_t> & departing_cars) {
  dijkstra(g, );
}

vector<intersection_id_t> getNextStep(Graph * graph,
                                      const std::vector<arrival_info_t> & arriving_cars);

void carArrived(PerCarInfo * finished_cars);

