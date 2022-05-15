#include <algorithm>
#include <functional>
#include <iostream>
#include <istream>
#include <numeric>
#include <queue>
#include <vector>

typedef int Vertice;

template <int indexing> class Edge;

/*
 * Structure representing an adjacent vertice, eg for adjacency list
 */
template <int indexing = 1> struct AdjacentVertice {
private:
  Edge<indexing> *edge;
  Vertice v;

public:
  AdjacentVertice(Edge<indexing> *edge, Vertice v) : edge(edge), v(v) {}

  AdjacentVertice() : edge(nullptr), v(-1) {}

  Vertice V() { return this->v; }

  Edge<indexing> *Edge() { return this->edge; }
};

/*
 * Base virtual edge class
 */
template <int indexing = 1> class Edge {
protected:
  Vertice from{-1}, to{-1};
  int id{-1};

public:
  virtual ~Edge() = default;

  template <class DerivedEdge>
  static DerivedEdge *readFrom(std::istream &in,
                               const std::function<int()> &idAllocator);

  [[nodiscard]] int Id() const;

  std::vector<Vertice> incidentVertices();

  virtual AdjacentVertice<indexing> adjacentTo(Vertice v) = 0;
};

template <int indexing>
template <class DerivedEdge>
DerivedEdge *Edge<indexing>::readFrom(std::istream &in,
                                      const std::function<int()> &idAllocator) {
  auto *edge = new DerivedEdge();
  in >> edge->from >> edge->to;
  edge->from -= indexing;
  edge->to -= indexing;
  edge->id = idAllocator();
  return edge;
}

template <int indexing> int Edge<indexing>::Id() const { return this->id; }

template <int indexing>
std::vector<Vertice> Edge<indexing>::incidentVertices() {
  return {this->from, this->to};
}

template <int indexing>
AdjacentVertice<indexing> Edge<indexing>::adjacentTo(Vertice v) {
  if (v == this->from) {
    return {this, this->to};
  }
  return {nullptr, -1};
}

/*
 * Directed edge variant
 */

template <int indexing> class DirectedEdge;

template <int indexing>
std::ostream &operator<<(std::ostream &out, const DirectedEdge<indexing> *edge);

template <int indexing = 1> class DirectedEdge : Edge<indexing> {
public:
  friend std::ostream &operator<<<>(std::ostream &out,
                                    const DirectedEdge *edge);
};

template <int indexing>
std::ostream &operator<<(std::ostream &out,
                         const DirectedEdge<indexing> *edge) {
  out << edge->from + 1 << ' ' << edge->to + 1;
  return out;
}

/*
 * Undirected edge variant
 */

template <int indexing> struct UndirectedEdge;

template <int indexing>
std::ostream &operator<<(std::ostream &out,
                         const UndirectedEdge<indexing> *edge);

template <int indexing = 1> struct UndirectedEdge : Edge<indexing> {
public:
  AdjacentVertice<indexing> adjacentTo(Vertice v) override;

  friend std::ostream &operator<<<>(std::ostream &out,
                                    const UndirectedEdge *edge);
};

template <int indexing>
AdjacentVertice<indexing> UndirectedEdge<indexing>::adjacentTo(Vertice v) {
  if (v == this->from) {
    return {this, this->to};
  } else if (v == this->to) {
    return {this, this->from};
  }
  return {nullptr, -1};
}

template <int indexing>
std::ostream &operator<<(std::ostream &out,
                         const UndirectedEdge<indexing> *edge) {
  out << edge->id;
  return out;
}

/*
 * Template class graph which takes an edge type
 */
template <class EdgeT, int indexing = 1> class Graph {
public:
  using AdjacentVertices = std::vector<AdjacentVertice<indexing>>;
  using AdjacencyList = std::vector<AdjacentVertices>;
  using EdgeVector = std::vector<EdgeT *>;
  using EdgeType = EdgeT;

private:
  AdjacencyList adjacencyList;
  EdgeVector edges;
  std::function<int()> edgeIdAllocator;

public:
  static Graph *readFrom(std::istream &in);

  Graph();

  ~Graph();

  int numOfVertices();

  std::vector<Vertice> vertices();

  AdjacentVertices &adjacentTo(Vertice v);
};

template <class EdgeT, int indexing>
Graph<EdgeT, indexing>::Graph() : adjacencyList(), edges() {
  edgeIdAllocator = [this]() -> int { return this->edges.size() + indexing; };
};

template <class EdgeT, int indexing> Graph<EdgeT, indexing>::~Graph() {
  for (auto edge : this->edges) {
    delete edge;
  }
}

template <class EdgeT, int indexing>
Graph<EdgeT, indexing> *Graph<EdgeT, indexing>::readFrom(std::istream &in) {
  auto *g = new Graph<EdgeT, indexing>();
  int n, m;
  in >> n >> m;
  g->adjacencyList.resize(n);
  g->edges.reserve(m);
  for (int i = 0; i < m; i++) {
    auto *edge = Edge<>::template readFrom<EdgeT>(in, g->edgeIdAllocator);
    g->edges.push_back(edge);
    for (Vertice &v : edge->incidentVertices()) {
      g->adjacencyList.at(v).push_back(edge->adjacentTo(v));
    }
  }
  return g;
}

template <class EdgeT, int indexing>
int Graph<EdgeT, indexing>::numOfVertices() {
  return this->adjacencyList.size();
}

template <class EdgeT, int indexing>
std::vector<Vertice> Graph<EdgeT, indexing>::vertices() {
  std::vector<Vertice> result(this->numOfVertices());
  std::iota(result.begin(), result.end(), 0);
  return result;
}

template <class EdgeT, int indexing>
typename Graph<EdgeT, indexing>::AdjacentVertices &
Graph<EdgeT, indexing>::adjacentTo(Vertice v) {
  return this->adjacencyList.at(v);
}

using UndiGraph = Graph<UndirectedEdge<>>;
using AdjacentVerticeT = AdjacentVertice<>;

using intVec = std::vector<int>;

void internalFindBridges(UndiGraph::EdgeVector *bridges, intVec &tin,
                         intVec &low, int &time, UndiGraph *g, Vertice v,
                         Vertice p) {
  tin[v] = low[v] = time++;
  for (AdjacentVerticeT u : g->adjacentTo(v)) {
    if (u.V() == p) {
      continue;
    }
    if (tin[u.V()] == -1) {
      internalFindBridges(bridges, tin, low, time, g, u.V(), v);
      low[v] = std::min(low[v], low[u.V()]);
      if (low[u.V()] > tin[v]) {
        bridges->push_back(dynamic_cast<UndiGraph::EdgeType *>(u.Edge()));
      }
    } else {
      low[v] = std::min(low[v], tin[u.V()]);
    }
  }
}

UndiGraph::EdgeVector *findBridges(UndiGraph *g) {
  intVec tin(g->numOfVertices(), -1);
  intVec low(g->numOfVertices(), -1);
  auto *bridges = new UndiGraph::EdgeVector();
  int time = 0;
  for (Vertice v : g->vertices()) {
    if (tin[v] == -1) {
      internalFindBridges(bridges, tin, low, time, g, v, -1);
    }
  }
  return bridges;
}

int main() {
  auto *g = UndiGraph::readFrom(std::cin);
  auto bridges = findBridges(g);
  intVec ids(bridges->size());
  for (unsigned int i = 0; i < bridges->size(); i++) {
    ids[i] = bridges->at(i)->Id();
  }
  std::sort(ids.begin(), ids.end());
  std::cout << ids.size() << std::endl;
  for (int id : ids) {
    std::cout << id << " ";
  }
  std::cout << std::endl;
  delete bridges;
  delete g;
}