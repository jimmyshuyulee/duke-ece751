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
  intersection_id_t destination;
  vector<intersection_id_t> path;

 public:
  PerCarInfo() = default;
  PerCarInfo(const unsigned & car_id,
             const vector<intersection_id_t> & car_path = vector<intersection_id_t>()) :
      id(car_id),
      path(car_path) {}
  unsigned getCarId() const { return id; }
  intersection_id_t getNextIntersectionId() {
    if (path.size() == 0) {
      return 0;
    }
    path.erase(path.begin());
    return path[0];
  }
  vector<intersection_id_t> getPath() const { return path; }
};

class RoadInfo {
 public:
  typedef pair<unsigned, float> road_time_info_t;
  unsigned id;
  intersection_id_t source;
  intersection_id_t destination;
  vector<road_time_info_t> road_time_info;

  RoadInfo() = default;
  RoadInfo(unsigned id_,
           intersection_id_t s,
           intersection_id_t d,
           vector<road_time_info_t> rti = vector<road_time_info_t>()) :
      id(id_),
      source(s),
      destination(d),
      road_time_info(rti) {}
};

// defined by student
// for now, this class is an alias for our GenericGraph
class Graph {
  typedef unordered_map<road_id_t, RoadInfo> adjacency_t;
  unsigned vertex_num;
  unsigned edge_num;
  vector<adjacency_t> g;

  // Did not think of a better wat to hash a from_to_pair_t, so I simply encode
  // the source-destination pair into a string
  unordered_map<std::string, vector<intersection_id_t> > shortest_path;

 public:
  Graph() = default;
  bool isInGraph(intersection_id_t i) { return g.size() > i; }
  adjacency_t getAdj(const intersection_id_t & idx) const { return g[idx]; }
  unsigned getVNum() const { return vertex_num; }
  vector<intersection_id_t> getShortestPath(const unsigned & s,
                                            const unsigned & d) const {
    // Encode source and destination pair into a string
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
    // Encode source and destination pair into a string
    std::string ftp = "from" + std::to_string(s) + "to" + std::to_string(d);
    shortest_path[ftp] = sp;
  }

  void addEdge(const unsigned & id,
               const unsigned & source,
               const unsigned & destination,
               const unsigned & length,
               vector<unsigned> & speed_carNum) {
    while (g.size() <= source || g.size() <= destination) {
      g.push_back(adjacency_t());
      ++vertex_num;
    }

    if (g[source].find(id) != g[source].end()) {
      std::cerr << "Exist multiple lines of information for the same road!" << std::endl;
      exit(EXIT_FAILURE);
    }

    RoadInfo ri(id, source, destination);
    for (unsigned i = 1; i < speed_carNum.size(); i += 2) {
      ri.road_time_info.emplace_back(speed_carNum[i],
                                     (float)length / speed_carNum[i - 1]);
    }
    g[source][id] = ri;
    ++edge_num;
  }

  void printGraph() {
    for (unsigned i = 1; i < g.size(); i++) {
      std::cout << "Edge " << i << ": " << std::endl;
      for (auto itr : g[i]) {
        std::cout << itr.first << " " << itr.second.source << " "
                  << itr.second.destination;
        for (unsigned k = 0; k < itr.second.road_time_info.size(); k++)
          std::cout << " " << itr.second.road_time_info[k].first << ":"
                    << itr.second.road_time_info[k].second;
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }
  }
};

vector<intersection_id_t> dijkstra(Graph * graph,
                                   const intersection_id_t & s,
                                   const intersection_id_t & d);

void getPath(vector<intersection_id_t> & path,
             const vector<intersection_id_t> & previous_node,
             const intersection_id_t & start,
             const intersection_id_t & end);

vector<intersection_id_t> dijkstraAtStart(Graph * graph,
                                          const intersection_id_t & s,
                                          const intersection_id_t & d);

vector<intersection_id_t> dijkstraAtIntersection(Graph * graph,
                                                 const intersection_id_t & s,
                                                 const intersection_id_t & d);

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

void cleanup(Graph * g);

#endif
