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
    if (str.empty()) {
      continue;  // Skip empty lines
    }
    str.erase(str.find_last_not_of(" ") + 1);  // Remove trailing whitespace
    std::size_t pos = 0;

    auto convert_and_push =
        [](vector<unsigned> & roadInfo, const std::string & s, const std::size_t & p) {
          try {
            roadInfo.push_back(stoi(s.substr(0, p)));
          }
          catch (std::invalid_argument) {
            std::cerr << "Graph file contains non-integer information" << std::endl;
            exit(EXIT_FAILURE);
          }
        };

    while ((pos = str.find(' ')) != std::string::npos) {
      convert_and_push(info, str, pos);
      str.erase(0, pos + 1);
    }
    convert_and_push(info, str, pos);

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
  return g;
}

vector<intersection_id_t> dijkstra(Graph * graph,
                                   const intersection_id_t & s,
                                   const intersection_id_t & d) {
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
  vector<intersection_id_t> pre(graph->getVNum(), 0);
  pre[s] = s;

  pq.push(std::make_pair(s, 0));
  while (!pq.empty()) {
    intersection_id_t curr = pq.top().first;
    pq.pop();
    for (auto road : graph->getAdj(curr)) {
      intersection_id_t neighbor_id = road.second.destination;
      // Set the travel time from curr to neighbor to infinity, and check
      // the number of cars on that road to decide the actual travel time.
      float travel_time = FLT_MAX;
#ifdef STEP2
      unsigned car_num =
          query_road(road.first).num_cars + query_road(road.first).num_pending_cars;
      for (auto road_time : road.second.road_time_info) {
        if (car_num < road_time.first) {
          travel_time = road_time.second;
          break;
        }
      }
#else
      travel_time = road.second.road_time_info[0].second;
#endif
      if (dist[neighbor_id] > dist[curr] + travel_time) {
        dist[neighbor_id] = dist[curr] + travel_time;
        pre[neighbor_id] = curr;
        pq.push(std::make_pair(neighbor_id, dist[neighbor_id]));
      }
    }
  }
  return pre;
}

void getPath(vector<intersection_id_t> & path,
             const vector<intersection_id_t> & previous_node,
             const intersection_id_t & start,
             const intersection_id_t & end) {
  intersection_id_t curr = end;
  while (curr != start) {
    if (curr == 0) {
      break;
    }
    path.push_back(curr);
    curr = previous_node[curr];
  }
  path.push_back(curr);
  std::reverse(path.begin(), path.end());
}

vector<intersection_id_t> dijkstraAtStart(Graph * graph,
                                          const intersection_id_t & s,
                                          const intersection_id_t & d) {
  vector<intersection_id_t> path = graph->getShortestPath(s, d);
  if (!path.empty()) {
    return path;
  }

  vector<intersection_id_t> pre(dijkstra(graph, s, d));

  // Save all the shortest path starting from source to graph
  for (unsigned i = 1; i < pre.size(); i++) {
    getPath(path, pre, s, i);
    graph->setShortestPath(s, i, path);
    path.clear();
  }
  return graph->getShortestPath(s, d);
}

#ifdef SEPT2
vector<intersection_id_t> dijkstraAtIntersection(Graph * graph,
                                                 const intersection_id_t & s,
                                                 const intersection_id_t & d) {
  vector<intersection_id_t> path;
  vector<intersection_id_t> pre(dijkstra(graph, s, d));
  getPath(path, pre, s, d);
  return path;
}
#endif

vector<PerCarInfo *> startPlanning(Graph * graph,
                                   const std::vector<start_info_t> & departing_cars) {
  vector<PerCarInfo *> ans;
  for (unsigned i = 0; i < departing_cars.size(); i++) {
    intersection_id_t source = departing_cars[i].second.first;
    intersection_id_t destination = departing_cars[i].second.second;
    if (!graph->isInGraph(source)) {
      std::cerr << "Source does not Exist!" << std::endl;
      exit(EXIT_FAILURE);
    }
    if (!graph->isInGraph(destination)) {
      std::cerr << "Destiantion does not Exist!" << std::endl;
      exit(EXIT_FAILURE);
    }

#ifdef STEP2
    PerCarInfo * car_info = new PerCarInfo(departing_cars[i].first);
#else
    PerCarInfo * car_info = new PerCarInfo(
        departing_cars[i].first,
        dijkstraAtStart(
            graph, departing_cars[i].second.first, departing_cars[i].second.second));
#endif
    ans.push_back(car_info);
  }
  return ans;
}

vector<intersection_id_t> getNextStep(Graph * graph,
                                      const std::vector<arrival_info_t> & arriving_cars) {
  vector<intersection_id_t> ans;
  for (unsigned i = 0; i < arriving_cars.size(); i++) {
#ifdef STEP2
    intersection_id_t next = dijkstraAtIntersection(graph, arriving_cars[i].second)[1];
#else
    intersection_id_t next = arriving_cars[i].second->getNextIntersectionId();
#endif
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

