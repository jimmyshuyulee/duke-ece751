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
    for (auto neighbor : graph->getAdj(curr)) {
      intersection_id_t neighbor_id = neighbor.first;
      int travel_time = neighbor.second[0].second;
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
    //std::cout << "success!\n";
  }
  return graph->getShortestPath(s, d);
}

vector<PerCarInfo *> startPlanning(Graph * graph,
                                   const std::vector<start_info_t> & departing_cars) {
  vector<PerCarInfo *> ans;
  for (int i = 0; i < departing_cars.size(); i++) {
    PerCarInfo * car_info = new PerCarInfo(
        departing_cars[i].first,
        dijkstra(g, departing_cars[i].second.first, departing_cars[i].second.second));
    ans.push_back(car_info);
  }
  return ans;
}

vector<intersection_id_t> getNextStep(Graph * graph,
                                      const std::vector<arrival_info_t> & arriving_cars);

void carArrived(PerCarInfo * finished_cars);

