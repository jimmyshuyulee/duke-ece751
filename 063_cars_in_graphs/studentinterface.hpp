#ifndef STUDENT_INTERFACE_HPP
#define STUDENT_INTERFACE_HPP
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "studentapi.hpp"

using std::pair;
using std::unordered_map;
using std::vector;

// defined by student
// should probably only need id and path...
class PerCarInfo {
  unsigned id;
  vector<intersection_id_t> path;

 public:
  PerCarInfo() = default;
  PerCarInfo(const unsigned & car_id, const vector<intersection_id_t> & car_path) :
      id(car_id),
      path(car_path) {}
  unsigned getCarId() const { return id; }
  intersection_id_t getNextIntersectionId(const intersection_id_t & inter_id) const {
    for (int i = 0; i < path.size() - 1; i++) {
      if (path[i] == inter_id) {
        return path[i + 1];
      }
      return 0;
    }
  }
  vector<intersection_id_t> getPath() const { return path; }
};

// defined by student
// for now, this class is an alias for our GenericGraph
class Graph {
  typedef pair<unsigned, float> road_time_info_t;
  typedef unordered_map<intersection_id_t, vector<road_time_info_t> > adjacency_t;
  unsigned vertex_num;
  unsigned edge_num;
  vector<adjacency_t> g;

  // Did not think of a better wat to hash a from_to_pair_t, so I simply encode
  // the source-destination pair into a string
  unordered_map<std::string, vector<intersection_id_t> > shortest_path;

 public:
  Graph() = default;
  adjacency_t getAdj(const intersection_id_t & idx) const { return g[idx]; }
  unsigned getVNum() const { return vertex_num; }
  vector<road_time_info_t> getRoadInfo(const intersection_id_t & s,
                                       const intersection_id_t & d) const {
    return g[s].at(d);
  }
  vector<intersection_id_t> getShortestPath(const unsigned & s,
                                            const unsigned & d) const {
    std::string ftp = "from" + std::to_string(s) + "to" + std::to_string(d);
    if (shortest_path.find(ftp) != shortest_path.end()) {
      return shortest_path.at(ftp);
    }
    else {
      return vector<intersection_id_t>();
    }
  }
  void setShortestPath(const unsigned & s,
                       const unsigned & d,
                       vector<intersection_id_t> sp) {
    std::string ftp = "from" + std::to_string(s) + "to" + std::to_string(d);
    shortest_path[ftp] = sp;
  }

  void addEdge(const unsigned & source,
               const unsigned & destination,
               const unsigned & length,
               vector<unsigned> & speed_carNum) {
    while (g.size() <= source || g.size() <= destination) {
      g.push_back(adjacency_t());
      ++vertex_num;
    }

    vector<road_time_info_t> cnt;
    for (int i = 1; i < speed_carNum.size(); i += 2) {
      cnt.emplace_back(speed_carNum[i], (float)length / speed_carNum[i - 1]);
    }
    g[source][destination] = cnt;
    ++edge_num;
  }

  void printGraph() {
    for (int i = 1; i < g.size(); i++) {
      std::cout << "Edge " << i << ": " << std::endl;
      for (auto itr : g[i]) {
        std::cout << itr.first;
        for (int k = 0; k < itr.second.size(); k++)
          std::cout << " " << itr.second[k].first << ":" << itr.second[k].second;
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }
  }
};

vector<intersection_id_t> dijkstra(Graph * graph,
                                   intersection_id_t s,
                                   intersection_id_t d);
// Creates the src, dest car pairs needed for startPlanning
Graph * readGraph(std::string fname);

//This will get called once for all cars departing at time T.  You will
//be passed a vector with the from/to intersection ids of each departing car.
//You should then return a vector of the same length with whatever state you want
//associated with each car.
//Note: this is called before the car "arrives" at its source intersection
std::vector<PerCarInfo *> startPlanning(Graph * graph,
                                        const std::vector<start_info_t> & departing_cars);

//whenever a car reaches an intersection (including the source, but
//excluding the destination), this function
//will be called to determine what the next intersection should be.
//If you specify an invalid id (not adjacent to the current one), the simulation
//will abort.
//
//Note that you could pre-plan in startPlanning, store the planned route
//in your PerCarInfo, and then just return the next step.
//
//However, you can also do something more interesting to adapt to changing
//traffic conditions
std::vector<intersection_id_t> getNextStep(
    Graph * graph,
    const std::vector<arrival_info_t> & arriving_cars);

//when a car reaches its destination, this function will be called.
//you may do any book-keeping you wish (which may be none)
//
//If your PerCarInfo is dynmically allocated, you may safely free it here
//Note: unlike the other two functions (which are called at most ONCE per cycle),
//this is called for each arriving car.
//[This design choice assumes you may wish to do coordinated planning, but
// that resource freeing and other book-keeping does not need coordination]
void carArrived(PerCarInfo * finished_cars);

#endif
