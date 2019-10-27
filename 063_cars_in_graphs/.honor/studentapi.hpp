#ifndef STUDENT_API_HPP
#define STUDENT_API_HPP
#include <vector>

class PerCarInfo;
typedef unsigned intersection_id_t;
typedef unsigned road_id_t;
typedef unsigned car_id_t;
typedef std::pair<intersection_id_t, PerCarInfo *> arrival_info_t;
typedef std::pair<intersection_id_t, intersection_id_t> from_to_pair_t;
typedef std::pair<car_id_t, from_to_pair_t> start_info_t;

class RoadStatus {
 public:
  road_id_t id;
  unsigned num_cars;          //how many cars are on this road
  unsigned num_pending_cars;  //how many cars are trying to enter
  RoadStatus() : id(-1), num_cars(-1), num_pending_cars(-1) {}
  RoadStatus(road_id_t rid, unsigned n_cars, unsigned n_pending) :
      id(rid),
      num_cars(n_cars),
      num_pending_cars(n_pending) {}
};

RoadStatus query_road(road_id_t id);
std::vector<RoadStatus> query_all_roads();

#endif
