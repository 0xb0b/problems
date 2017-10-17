#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>

using std::vector;
using std::pair;
using std::make_pair;

extern const long long infinity;

struct Node
{
  size_t from_; // used for O(1) initialization
  size_t to_; // used for O(1) initialization
  vector<pair<int, int> > neighbors; // pair<edge length, vertex>
  long long distance {infinity}; // shortest distance from the source node
  Node* prev {nullptr}; // previous node on the shortest path
  bool visited {false}; // if the node has been visited in the search
};

class Graph
{
  vector<Node> adjList_; // adjacency list of the graph
  size_t top_; // used for O(1) initialization
  int size_; // number of the nodes in the graph

public:
  explicit Graph(int size);
  Graph(const Graph& src, bool reversed = false);
  Graph(int min_size, int max_size, double max_density,
        int min_edge_length, int max_edge_length);
  Graph& operator=(const Graph& rhs) = delete;
  Node& operator[](const int index);
  const Node& operator[](const int index) const;
  int size() const;
  friend std::ostream& operator<<(std::ostream& output, const Graph& graph);
  void AddEdge(int start, int end, int length);
  void Reset();
};

std::ostream& operator<<(std::ostream& output, const Graph& graph);

#endif // GRAPH_H
