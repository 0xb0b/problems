#include <random>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iostream>
#include "graph.h"

extern const long long infinity = std::numeric_limits<long long>::max();

Graph::Graph(int size): size_ {size}, top_ {0}
{
  adjList_.resize(size_);
}

Graph::Graph(const Graph& src, bool reversed): Graph(src.size_)
{
  // if reversed is true construct reversed graph with the edges in
  // the opposite direction
  // graph vertices are numbered 1-based but stored in zero-based array
  for (int u = 1; u <= size_; ++u)
  {
    for (const auto& neighbor : src[u].neighbors)
    {
      int length = neighbor.first;
      int v = neighbor.second;
      if (reversed)
      {
        adjList_.at(v - 1).neighbors.push_back(make_pair(length, u));
      }
      else
      {
        adjList_.at(u - 1).neighbors.push_back(neighbor);
      }
    }
  }
}

Graph::Graph(int min_size, int max_size, double max_density,
             int min_edge_length, int max_edge_length)
{
  std::random_device rand_dev;
  std::mt19937 random;
  random.seed(rand_dev());
  std::uniform_int_distribution<int> uniform_size(min_size, max_size);
  size_ = uniform_size(random);
  top_ = 0;
  adjList_.resize(size_);
  int max_numedges {static_cast<int>(std::round(max_density * size_))};
  std::uniform_int_distribution<int> uniform_numedges(0, max_numedges);
  std::uniform_int_distribution<int> uniform_length(min_edge_length,
                                                    max_edge_length);
  vector<int> node_indices(size_);
  std::iota(node_indices.begin(), node_indices.end(), 1);
  for (int u = 0; u < size_; ++u)
  {
    std::shuffle(node_indices.begin(), node_indices.end(), random);
    int numedges = uniform_numedges(random);
    for (int i = 0; i < numedges; ++i)
    {
      int edge_length = uniform_length(random);
      int v = node_indices.at(i);
      if (v != u + 1)
      {
        adjList_.at(u).neighbors.push_back(make_pair(edge_length, v));
      }
    }
  }
}

int Graph::size() const
{
  return size_;
}

Node& Graph::operator[](const int index)
{
  // graph vertices are numbered 1-based but stored in zero-based array
  int i = index - 1;
  Node& element = adjList_.at(i);
  size_t from = element.from_;
  if (from < top_)
  {
    size_t to = adjList_.at(from).to_;
    if (to == i)
    {
      return element;
    }
  }
  element.from_ = top_;
  adjList_.at(top_).to_ = i;
  element.distance = infinity;
  element.prev = nullptr;
  element.visited = false;
  ++top_;
  return element;
}

const Node& Graph::operator[](const int index) const
{
  // graph vertices are numbered 1-based but stored in zero-based array
  return adjList_.at(index - 1);
}

void Graph::AddEdge(int start, int end, int length)
{
  // graph vertices are numbered 1-based but stored in zero-based array
  adjList_.at(start - 1).neighbors.push_back(make_pair(length, end));
}

void Graph::Reset()
{
  top_ = 0;
}

std::ostream& operator<<(std::ostream& output, const Graph& graph)
{
  output << "graph: {" << std::endl;
  for (int u = 1; u <= graph.size_; ++u)
  {
    output << "[" << u << "]: ";
    const Node& node = graph[u];
    for (const auto& neighbor : node.neighbors)
    {
      output << " (" << neighbor.first << ") " << neighbor.second << ", ";
    }
    output << std::endl;
  }
  output << "}" << std::endl;
  return output;
}

