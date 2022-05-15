#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <iterator>
#include <queue>
#include <stack>
#include <unordered_set>
#include <vector>

using uint64 = uint64_t;
using uint32 = uint32_t;
using int64 = int64_t;
using int32 = int32_t;

template <typename T> using Vec = std::vector<T>;

class Range {
  const int from;
  const int to;

public:
  class iterator {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = const int;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type *;
    using reference = const value_type &;

  private:
    const int from;
    const int to;
    int i;

  public:
    iterator(const int from, const int to, const int i)
        : from(from), to(to), i(i){};
    iterator &operator++() {
      i = to >= from ? i + 1 : i - 1;
      return *this;
    }
    iterator operator++(int) {
      iterator ret = *this;
      ++(*this);
      return ret;
    }
    bool operator==(const iterator &other) const { return i == other.i; }
    bool operator!=(const iterator &other) const { return i != other.i; }
    reference operator*() const { return i; }
  };

  explicit Range(const int to) : from(0), to(to) {}
  Range(const int from, const int to) : from(from), to(to) {}

  [[nodiscard]] iterator begin() const { return iterator(from, to, from); }
  [[nodiscard]] iterator end() const { return iterator(from, to, to); }
};

class IO {
  std::istream &input;
  std::ostream &output;

public:
  IO(std::istream &in, std::ostream &out) : input(in), output(out) {
    input.tie(nullptr);
    output.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
  }

  template <typename T> auto ReadT() const {
    return [this]() {
      T v;
      input >> v;
      return v;
    };
  }

  template <typename T> [[nodiscard]] T read() const {
    T v;
    input >> v;
    return v;
  }

  template <typename T> void read(T &v) const { input >> v; }

  template <typename T> auto WriteT() const {
    return [this](const T &v) { output << v; };
  }

  template <typename T> void write(const T &v) const { output << v; }

  void nl() const { output << std::endl; }
  void sp() const { output << ' '; }
};

class DSU {
public:
  explicit DSU(const int n) : parent(n), size(n) {
    for (const int i : Range(n)) {
      parent[i] = i;
      size[i] = 1;
    }
  }

  int findRoot(const int v) {
    if (v == parent[v]) {
      return v;
    }
    return parent[v] = findRoot(parent[v]);
  }

  void unite(const int a, const int b) {
    int rootA = findRoot(a);
    int rootB = findRoot(b);
    if (rootA != rootB) {
      if (size[rootA] < size[rootB]) {
        std::swap(rootA, rootB);
      }
      parent[rootB] = rootA;
      size[rootA] += size[rootB];
    }
  }

  bool areInSameSet(const int a, const int b) {
    return findRoot(a) == findRoot(b);
  }

  bool areInDifferentSets(const int a, const int b) {
    return findRoot(a) != findRoot(b);
  }

private:
  Vec<int> parent;
  Vec<int> size;
};

// transition from one vertex to a different one over some edge
struct EdgeTransition {
  int to;
  int edgeId;

  EdgeTransition() : to(-1), edgeId(-1) {}
  EdgeTransition(const int to, const int edgeId) : to(to), edgeId(edgeId) {}

  // needed for hashset of adjacent vertices we can simply
  // compare and hash by edgeId since it's *supposed* to be unique
  bool operator==(const EdgeTransition &other) const {
    return this->edgeId == other.edgeId;
  }
  struct Hasher {
    bool operator()(const EdgeTransition &transition) const {
      return std::hash<int>()(transition.edgeId);
    }
  };
};

// edge with an id in an undirected graph
struct Edge {
  int u;
  int v;
  int id;

  Edge() : u(-1), v(-1), id(-1) {}
  Edge(const int u, const int v, const int id) : u(u), v(v), id(id) {}

  [[nodiscard]] EdgeTransition transitionFrom(const int x) const {
    if (x == u) {
      return {v, id};
    } else if (x == v) {
      return {u, id};
    }
    throw std::runtime_error("traversing edge from unknown vertice");
  }
};

// ParentTransition is an EdgeTransition where to is vertex' parent
// and edgeId is the id of the edge from parent to vertex
using ParentTransition = EdgeTransition;

using EdgeList = Vec<Edge>;
// representation of the forest edges
using EdgeSet = std::unordered_set<int>;
// hashmap <edge id, to> for fast add/remove by id
using Adjacent = std::unordered_set<EdgeTransition, EdgeTransition::Hasher>;
using AdjacencyList = Vec<Adjacent>;

// representation of a single forest encapsulating all operations
// which we will need to do with each forest during the STP algorithm
class Forest {
public:
  explicit Forest(const int numVertices)
      : graph(numVertices), dsu(numVertices) {}

  void removeEdge(const Edge &edge) {
    const auto [u, v, id] = edge;
    edges.erase(id);

    graph.at(u).erase(edge.transitionFrom(u));
    graph.at(v).erase(edge.transitionFrom(v));
  }

  void addEdge(const Edge &edge) {
    const auto [u, v, id] = edge;
    edges.emplace(id);

    graph.at(u).emplace(edge.transitionFrom(u));
    graph.at(v).emplace(edge.transitionFrom(v));
  }

  bool isInDifferentTrees(const Edge &edge) {
    return dsu.areInDifferentSets(edge.u, edge.v);
  }

  void joinTreesByEdge(const Edge &edge) { dsu.unite(edge.u, edge.v); }

  void traverseFromVertex(Vec<ParentTransition> &parent, const int v) {
    for (const auto &transition : graph.at(v)) {
      auto &curParent = parent.at(transition.to);
      if (curParent.to == -1) {
        curParent.to = v;
        curParent.edgeId = transition.edgeId;
        traverseFromVertex(parent, transition.to);
      }
    }
  }

  [[nodiscard]] const EdgeSet &getEdges() const { return edges; }

  [[nodiscard]] size_t size() const { return edges.size(); }

private:
  // list of used edges, stored for fast output
  EdgeSet edges;
  // a graph representation of the forest for dfs and the like
  AdjacencyList graph;
  // dsu representation of the forest for fast merging
  // and finding tree representatives
  DSU dsu;
};

// class encapsulating the logic of the spanning tree packing algo
class SpanningTreePacking {
public:
  SpanningTreePacking(const int numVertices, EdgeList &&edges)
      : numVertices(numVertices), edges(std::move(edges)),
        edgeForestIndex(this->edges.size(), -1) {}

  // pack runs the algorithm
  Vec<std::reference_wrapper<const EdgeSet>> pack() {
    for (const Edge &edge : edges) {
      augmentWithEdge(edge);
    }
    Vec<std::reference_wrapper<const EdgeSet>> spanningTrees{};
    spanningTrees.reserve(forests.size());
    for (const Forest &forest : forests) {
      if (int(forest.size()) != numVertices - 1) {
        break;
      }
      spanningTrees.emplace_back(forest.getEdges());
    }
    return spanningTrees;
  }

private:
  struct PackingState {
    PackingState(const Edge &edge, const int numEdges, const int numForests)
        : k(0), x(edge.u), edge(edge), edgeLabels(numEdges, -1),
          nextStepInitEdges(1, edge.id) {
      parents.reserve(numForests);
    }

    // number of forests we are currently taking into account
    int k;
    // vertex on edge from which we are starting
    const int x;
    // edge which we are trying to add to the forests
    const Edge &edge;
    // parent lists for each forest gotten by traversing them from edge
    Vec<Vec<ParentTransition>> parents;
    // edgeLabels[i] is the id of the edge which will need to be added
    // to forest edgeForestIndex[i] after removing this edge
    Vec<int> edgeLabels;
    // ids of edges which should be used during the next step
    // for initialization of the queue. specifically these are the edges
    // labeled in forest k, since only it can be used to expand the
    // augmenting path in the next step with k+1 forests
    Vec<int> nextStepInitEdges;
  };

  // expandForStepK creates new forests if k > current number of forests
  // and calculates new parent arrays for state
  void expandForStepK(PackingState &state, const int k) {
    if (k <= state.k) {
      throw std::runtime_error("somehow decreased k during stp");
    }

    // create new forests if needed
    for (int i = int(forests.size()); i < k; i++) {
      forests.emplace_back(numVertices);
    }

    // initialize parent arrays for new forests
    for (int i = state.k; i < k; i++) {
      auto &parent =
          state.parents.emplace_back(numVertices, ParentTransition{-1, -1});
      parent.at(state.x) = {state.x, -1};
      forests.at(i).traverseFromVertex(parent, state.x);
    }

    state.k = k;
  }

  // single augmentation step for k forests
  bool tryAugment(PackingState &state) {
    // initialize q with either edge.id or all edge ids from
    // previous augmentation steps labeled on k-1'th forest
    std::queue<int> q{};
    for (const int edgeId : state.nextStepInitEdges) {
      q.push(edgeId);
    }

    // resize(0) instead of clear() to avoid deallocation
    state.nextStepInitEdges.resize(0);

    while (!q.empty()) {
      const auto &edge = edges.at(q.front());
      q.pop();

      // forest to which we will be adding edge if we find an augmenting path
      int nextForestIndex = (edgeForestIndex.at(edge.id) + 1) % state.k;
      std::reference_wrapper<Forest> nextForestRef =
          forests.at(nextForestIndex);

      if (nextForestRef.get().isInDifferentTrees(edge)) {
        // edge is in different trees, this means we can join them
        // thus augmenting the current solution (added a new edge to it)
        nextForestRef.get().joinTreesByEdge(edge);

        // remove edge from its current tree, add it to the next one
        // and repeat while we have some other edge this one depends on
        // needed so that we don't create cycles
        int pathEdgeId = edge.id;
        while (state.edgeLabels.at(pathEdgeId) != -1) {
          const auto &pathEdge = edges.at(pathEdgeId);
          const std::reference_wrapper<Forest> forestRef =
              forests.at(edgeForestIndex.at(pathEdge.id));

          forestRef.get().removeEdge(pathEdge);
          nextForestRef.get().addEdge(pathEdge);

          // set index of current edge to nextForestIndex
          // and nextForestIndex to the current forest index
          // so that the next edge replaces this one in the current tree
          std::swap(edgeForestIndex.at(pathEdge.id), nextForestIndex);
          nextForestRef = std::reference_wrapper(forests.at(nextForestIndex));
          // next edge is the label of this one
          pathEdgeId = state.edgeLabels.at(pathEdge.id);
        }
        const auto &pathEdge = edges.at(pathEdgeId);
        nextForestRef.get().addEdge(pathEdge);
        edgeForestIndex.at(pathEdge.id) = nextForestIndex;
        return true;
      } else {
        // traverse the tree which contains edge's vertices
        // and label all edges on the path between them
        const auto &parent = state.parents.at(nextForestIndex);
        int labelPathV = -1;
        if (edge.u == state.x ||
            state.edgeLabels.at(parent.at(edge.u).edgeId) != -1) {
          labelPathV = edge.v;
        } else {
          labelPathV = edge.u;
        }

        std::stack<int> s{};
        while (labelPathV != state.x &&
               state.edgeLabels.at(parent.at(labelPathV).edgeId) == -1) {
          const auto &vParent = parent.at(labelPathV);
          s.push(vParent.edgeId);
          labelPathV = vParent.to;
        }

        while (!s.empty()) {
          const int edgeId = s.top();
          s.pop();
          state.edgeLabels.at(edgeId) = edge.id;
          q.push(edgeId);
          // if we are labeling edges of the last currently available forest
          // make sure we add them as init edges for next step with k+1 forests
          if (nextForestIndex + 1 == state.k) {
            state.nextStepInitEdges.emplace_back(edgeId);
          }
        }
      }
    }
    return false;
  }

  void augmentWithEdge(const Edge &edge) {
    PackingState state(edge, int(edges.size()), int(forests.size()));
    int usedForests = 1;
    bool augmented = false;
    while (!augmented) {
      expandForStepK(state, usedForests);
      augmented = tryAugment(state);
      usedForests++;
    }
  }

  // static values representing the graph we are working with
  const int numVertices;
  const EdgeList edges;

  // memoized values required during each algorithm step
  // all of the created forests
  Vec<Forest> forests;
  // mapping edgeId -> forest index
  Vec<int> edgeForestIndex;
};

int main() {
  freopen("multispan.in", "r", stdin);
  freopen("multispan.out", "w", stdout);

  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
  const auto io = IO(std::cin, std::cout);

  const auto n = io.read<int>();
  const auto m = io.read<int>();
  EdgeList edges(m);
  for (const int i : Range(m)) {
    edges.at(i).u = io.read<int>() - 1;
    edges.at(i).v = io.read<int>() - 1;
    edges.at(i).id = i;
  }

  SpanningTreePacking packing(n, std::move(edges));
  auto trees = packing.pack();

  io.write(trees.size());
  io.nl();
  for (const EdgeSet &tree : trees) {
    for (const auto edgeId : tree) {
      io.write(edgeId + 1);
      io.sp();
    }
    io.nl();
  }
}