#include <limits>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <cstdlib>


const int INFINITY = std::numeric_limits<int>::max();

struct MapT {
  const unsigned char* data;
  int width;
  int size;
};

struct NodeValue {
  int index;
  int f;
  int h;

  NodeValue(int index, int f_value, int heuristic) :
    index {index}, f {f_value}, h {heuristic} { }

  bool operator>(const NodeValue& other) const {
    return f > other.f || (f == other.f && h > other.h);
  }
};

using NodesHeap = std::priority_queue<NodeValue,
                                      std::vector<NodeValue>,
                                      std::greater<NodeValue> >;

struct NodeInfo {
  int g;
  int h;
  int f;
  int previous;
  bool visited;

  NodeInfo(int g_value, int heuristic, int prev_index) : g {g_value},
                                                         h {heuristic},
                                                         previous {prev_index},
                                                         visited {false} {
    if (g < INFINITY) {
      f = g + h;
    }
    else {
      f = INFINITY;
    }
  }
};

using NodesMap = std::unordered_map<int, NodeInfo>;


int Coordinates2Index(int x, int y, int mapwidth) {
  return x + mapwidth * y;
}


std::vector<int> GetNeighbors2Explore(int node_index, const MapT& map) {
  std::vector<int> neighbors;
  neighbors.reserve(4);
  auto index = node_index - map.width;
  if (index >= 0 && map.data[index]) {
    neighbors.push_back(index);
  }
  index = node_index + map.width;
  if (index < map.size && map.data[index]) {
    neighbors.push_back(index);
  }
  index = node_index - 1;
  if (index >= 0 && index % map.width != map.width - 1 && map.data[index]) {
    neighbors.push_back(index);
  }
  index = node_index + 1;
  if (index < map.size && index % map.width != 0 && map.data[index]) {
    neighbors.push_back(index);
  }
  return neighbors;
}


bool MeetInTheMiddle(NodesHeap& fringe, NodesMap& explored,
                     const NodesMap& explored_reversed, const MapT& map,
                     int startx, int starty, int targetx, int targety) {
  auto node_index = fringe.top().index;
  fringe.pop();
  while (explored.at(node_index).visited) {
    if (fringe.empty()) {
      return false;
    }
    node_index = fringe.top().index;
    fringe.pop();
  }
  auto& node = explored.at(node_index);
  for (const auto& neighbor_index : GetNeighbors2Explore(node_index, map)) {
    if (explored.find(neighbor_index) == explored.end()) {
      int neighborx = neighbor_index % map.width;
      int neighbory = neighbor_index / map.width;
      int forward_h = std::abs(neighborx - targetx) +
                      std::abs(neighbory - targety);
      int bckward_h = std::abs(neighborx - startx) +
                      std::abs(neighbory - starty);
      int heuristic = forward_h - bckward_h;
      explored.insert(
          {neighbor_index, NodeInfo(INFINITY, heuristic, node_index)});
    }
    auto& neighbor = explored.at(neighbor_index);
    int path_length = node.g + 2;
    if (path_length < neighbor.g) {
      neighbor.g = path_length;
      neighbor.f = path_length + neighbor.h;
      neighbor.previous = node_index;
      fringe.push({neighbor_index, neighbor.f, neighbor.h});
    }
  }
  node.visited = true;
  return explored_reversed.find(node_index) != explored_reversed.end();
}


int FindPath(const int startx, const int starty,
             const int targetx, const int targety, 
             const unsigned char* map_ptr,
             const int mapwidth, const int mapheight,
             int* outbuffer_ptr, const int outbuffer_size) {

  MapT map {map_ptr, mapwidth, mapwidth * mapheight};
  auto start_index = Coordinates2Index(startx, starty, mapwidth);
  auto target_index = Coordinates2Index(targetx, targety, mapwidth);
  NodesMap explored_forward({ {start_index, NodeInfo(0, 0, -1)} });
  NodesMap explored_bckward({ {target_index, NodeInfo(0, 0, -1)} });
  NodesHeap forward_fringe;
  forward_fringe.push({start_index, 0, 0});
  NodesHeap bckward_fringe;
  bckward_fringe.push({target_index, 0, 0});

  while (!forward_fringe.empty() && !bckward_fringe.empty()) {
    if (MeetInTheMiddle(forward_fringe, explored_forward, explored_bckward, map,
                        startx, starty, targetx, targety)
        ||
        MeetInTheMiddle(bckward_fringe, explored_bckward, explored_forward, map,
                        targetx, targety, startx, starty)
        ) {
      // reconstruct the shortest path
      int shortest_distance = INFINITY;
      int midpoint;
      for (const auto& entry : explored_forward) {
        auto index = entry.first;
        auto node = entry.second;
        auto bckward_iter = explored_bckward.find(index);
        if (bckward_iter != explored_bckward.end()) {
          auto bckward_node = bckward_iter->second;
          if (node.visited || bckward_node.visited) {
            shortest_distance =
              std::min(shortest_distance, node.g + bckward_node.g);
            midpoint = index;
          }
        }
      }
      std::vector<int> mid_to_start;
      int node_index = midpoint;
      while (node_index != start_index) {
        mid_to_start.push_back(node_index);
        node_index = explored_forward.at(node_index).previous;
      }
      int out_index = 0;
      for (int i = mid_to_start.size() - 1; i >= 0; i--) {
        if (out_index < outbuffer_size) {
          outbuffer_ptr[out_index] = mid_to_start[i];
          ++out_index;
        }
        else {
          break;
        }
      }
      node_index = midpoint;
      while (node_index != target_index && out_index < outbuffer_size) {
        node_index = explored_bckward.at(node_index).previous;
        outbuffer_ptr[out_index] = node_index;
        ++out_index;
      }
      return shortest_distance / 2;
    }
  }
  return -1;
}

