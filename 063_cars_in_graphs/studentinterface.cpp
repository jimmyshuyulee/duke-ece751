#include "studentinterface.hpp"

#include <algorithm>
#include <cfloat>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <utility>

using std::pair;
using std::vector;

Graph * readGraph(std::string fname) {
  Graph * g = new Graph();
  std::ifstream ifs;
  ifs.open(fname);
  std::string str;
  vector<unsigned> info;

  while (!ifs.eof()) {
    std::getline(ifs, str);
    str.erase(str.find_last_not_of(" ") + 1);  // Remove trailing whitespace
    std::size_t pos = 0;
    while ((pos = str.find(' ')) != std::string::npos) {
      info.push_back(stoi(str.substr(0, pos)));
      str.erase(0, pos + 1);
    }

    try {
      info.push_back(stoi(str.substr(0, pos)));
    }
    catch (std::invalid_argument) {
      std::cout << str.substr(0, pos) << "aaaa";
    }
    // The minimun number of argument per line (per road) should be 6,
    // and the number of argument per line should be an even number
    if (info.size() < 6 || info.size() % 2 != 0) {
      std::cerr << "The format of road information is incorrect" << std::endl;
      exit(EXIT_FAILURE);
    }

    unsigned id = info[0];
    info.erase(info.begin());
    unsigned source = info[0];
    info.erase(info.begin());
    unsigned destination = info[0];
    info.erase(info.begin());
    unsigned length = info[0];
    info.erase(info.begin());
    unsigned maxcars = info[0];
    info.erase(info.begin());

    info.push_back(maxcars);
    g->addEdge(id, source, destination, length, info);
    info.clear();
  }
  //g->printGraph();
  return g;
}

vector<intersection_id_t> dijkstra(Graph * graph,
                                   intersection_id_t s,
                                   intersection_id_t d) {
  vector<intersection_id_t> path = graph->getShortestPath(s, d);
  if (!path.empty()) {
    return path;
  }

  auto my_comp = [](const pair<intersection_id_t, float> & x,
                    const pair<intersection_id_t, float> & y) {
    return x.second < y.second;
  };
  std::priority_queue<pair<intersection_id_t, float>,
                      vector<pair<intersection_id_t, float> >,
                      decltype(my_comp)>
      pq(my_comp);
  vector<float> dist(graph->getVNum(), FLT_MAX);
  dist[s] = 0;
  vector<int> pre(graph->getVNum(), 0);
  pre[s] = s;

  pq.push(std::make_pair(s, 0));
  while (!pq.empty()) {
    intersection_id_t curr = pq.top().first;
    pq.pop();
    for (auto road : graph->getAdj(curr)) {
      intersection_id_t neighbor_id = road.second.destination;
      int travel_time = road.second.road_time_info[0].second;
      if (dist[neighbor_id] > dist[curr] + travel_time) {
        dist[neighbor_id] = dist[curr] + travel_time;
        pre[neighbor_id] = curr;
        pq.push(std::make_pair(neighbor_id, dist[neighbor_id]));
      }
    }
  }

  for (unsigned i = 1; i < pre.size(); i++) {
    intersection_id_t curr = i;
    while (curr != s) {
      if (curr == 0) {
        break;
      }
      path.push_back(curr);
      curr = pre[curr];
    }
    if (curr == 0) {
      continue;
    }
    path.push_back(curr);
    std::reverse(path.begin(), path.end());
    graph->setShortestPath(curr, i, path);
    path.clear();
  }
  return graph->getShortestPath(s, d);
}

vector<PerCarInfo *> startPlanning(Graph * graph,
                                   const std::vector<start_info_t> & departing_cars) {
  vector<PerCarInfo *> ans;
  for (unsigned i = 0; i < departing_cars.size(); i++) {
    PerCarInfo * car_info = new PerCarInfo(
        departing_cars[i].first,
        dijkstra(graph, departing_cars[i].second.first, departing_cars[i].second.second));
    ans.push_back(car_info);
  }
  return ans;
}

vector<intersection_id_t> getNextStep(Graph * graph,
                                      const std::vector<arrival_info_t> & arriving_cars) {
  vector<intersection_id_t> ans;
  for (unsigned i = 0; i < arriving_cars.size(); i++) {
    intersection_id_t next =
        arriving_cars[i].second->getNextIntersectionId(arriving_cars[i].first);
    if (next == 0) {
      std::cerr << "Next step does not Exist!" << std::endl;
      exit(EXIT_FAILURE);
    }
    ans.push_back(next);
  }
  return ans;
}

void carArrived(PerCarInfo * finished_cars) {
  delete finished_cars;
}

