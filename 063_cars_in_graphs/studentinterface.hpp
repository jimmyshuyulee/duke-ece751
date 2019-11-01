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
class PerCarInfo {};

// defined by student
// for now, this class is an alias for our GenericGraph
class Graph {
  class RoadInfo {
   public:
    unsigned carLimit;
    vector<float> totalspeed_carNum;

    RoadInfo() = default;
    RoadInfo(const unsigned & car, vector<float> & speed_carNum) :
        carLimit(car),
        totalspeed_carNum(speed_carNum) {}
  };
  typedef pair<unsigned, float> road_info_t;
  typedef unordered_map<intersection_id_t, vector<road_info_t> > adjacency_t;

  vector<adjacency_t> g;

 public:
  Graph() = default;
  void addEdge(const unsigned & source,
               const unsigned & destination,
               const unsigned & length,
               vector<unsigned> & speed_carNum) {
    while (g.size() <= source) {
      g.push_back(adjacency_t());
    }

    for (int i = 1; i < speed_carNum.size(); i += 2) {
      g[source][destination].push_back(
          road_info_t(speed_carNum[i], (float)length / speed_carNum[i - 1]));
    }
  }

  void printGraph() {
    for (int i = 0; i < g.size(); i++) {
      std::cout << "Edge " << i << ": " << std::endl;
      for (adjacency_t::iterator itr = g[i].begin(); itr != g[i].end(); itr++) {
        std::cout << itr->first;
        for (int j = 0; j < itr->second.size(); j++) {
          std::cout << " " << itr->second[j].first << ":" << itr->second[j].second;
        }
        std::cout << std::endl;
      }
    }
  }
};

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
