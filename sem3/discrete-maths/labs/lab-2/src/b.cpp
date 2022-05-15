#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using uint64 = uint64_t;
using uint32 = uint32_t;
using int64 = int64_t;
using int32 = int32_t;

template <typename T> using Vec = std::vector<T>;
using IntVec = Vec<int>;

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

  Range(const int to) : from(0), to(to) {}
  Range(const int from, const int to) : from(from), to(to) {}

  iterator begin() { return iterator(from, to, from); }
  iterator end() { return iterator(from, to, to); }
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

  template <typename T> T read() const {
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

class Edge {
public:
  int from = -1;
  int to = -1;
  int64 weight = 0;
  int id = -1;
  bool used = false;
};

class DSU {
public:
  DSU(int n) : parent(n), size(n) {
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
  IntVec parent;
  IntVec size;
};

int64 maxSpanningTreeWeight(const int n, std::vector<Edge> &edges) {
  DSU dsu(n);

  int64 weight = 0;
  for (auto &edge : edges) {
    if (dsu.areInDifferentSets(edge.from, edge.to)) {
      weight += edge.weight;
      dsu.unite(edge.from, edge.to);
      edge.used = true;
    }
  }
  return weight;
}

int main() {
  freopen("destroy.in", "r", stdin);
  freopen("destroy.out", "w", stdout);

  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
  const auto io = IO(std::cin, std::cout);

  const auto n = io.read<int>();
  const auto m = io.read<int>();
  const auto s = io.read<int64>();
  std::vector<Edge> edges(m);

  uint64 sumOfAllEdges = 0;
  for (const int i : Range(m)) {
    edges[i].from = io.read<int>() - 1;
    edges[i].to = io.read<int>() - 1;
    edges[i].weight = io.read<int64>();
    edges[i].id = i + 1;
    edges[i].used = false;

    sumOfAllEdges += edges[i].weight;
  }

  // sort edges from max weight to min weight
  // this is needed for max spanning tree via kruskal
  // as well as
  std::sort(edges.begin(), edges.end(),
            [](const Edge &a, const Edge &b) { return a.weight > b.weight; });

  int64 usedEdgeWeight = maxSpanningTreeWeight(n, edges);

  for (auto &edge : edges) {
    if (usedEdgeWeight + s >= sumOfAllEdges) {
      break;
    }
    if (!edge.used) {
      edge.used = true;
      usedEdgeWeight += edge.weight;
    }
  }

  std::vector<int> selectedEdges;
  selectedEdges.reserve(m);
  for (const auto &edge : edges) {
    if (!edge.used) {
      selectedEdges.emplace_back(edge.id);
    }
  }
  std::sort(selectedEdges.begin(), selectedEdges.end());

  io.write(selectedEdges.size());
  io.nl();
  for (const auto edgeIndex : selectedEdges) {
    io.write(edgeIndex);
    io.sp();
  }
  io.nl();
}