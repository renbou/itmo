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

using Adjacent = Vec<Vec<bool>>;
using Graph = Vec<Vec<int>>;

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

struct Vertex {
  int id;
  int64 weight;
};

// kuhn's single run trying to find an augmenting path
// since we launch the search with weights in sorted order,
// we will always first select the vertices which weigh more
bool dfsKuhn(Graph &g, IntVec &match, IntVec &reverseMatch,
             std::vector<bool> &used, int v) {
  if (used[v]) {
    return false;
  }
  used[v] = true;
  for (const int to : g[v]) {
    if (match[to] == -1 || dfsKuhn(g, match, reverseMatch, used, match[to])) {
      match[to] = v;
      reverseMatch[v] = to;
      return true;
    }
  }
  return false;
}

int main() {
  freopen("matching.in", "r", stdin);
  freopen("matching.out", "w", stdout);

  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
  const auto io = IO(std::cin, std::cout);

  const int n = io.read<int>();
  std::vector<Vertex> vertices(n);
  for (const int i : Range(n)) {
    io.read(vertices[i].weight);
    vertices[i].id = i;
  }

  Graph g(n);
  for (const int i : Range(n)) {
    const int numIncidentEdges = io.read<int>();
    for (const int _ : Range(numIncidentEdges)) {
      g[i].emplace_back(io.read<int>() - 1);
    }
  }

  // sort by decreasing weight for Rado-Edmunds greedy algorithm
  std::sort(
      vertices.begin(), vertices.end(),
      [](const Vertex &a, const Vertex &b) { return a.weight > b.weight; });
  IntVec match(n, -1);
  IntVec reverseMatch(n, -1);
  auto used = std::vector<bool>(n);
  for (const Vertex &v : vertices) {
    std::fill(used.begin(), used.end(), false);
    dfsKuhn(g, match, reverseMatch, used, v.id);
  }

  for (const int m : reverseMatch) {
    io.write(m + 1);
    io.sp();
  }
  io.nl();
}