#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using uint64 = uint64_t;
using uint32 = uint32_t;
using int64 = int64_t;
using int32 = int32_t;

template <typename T> using Vec = std::vector<T>;
template <typename T, typename U> using Pair = std::pair<T, U>;
using IntVec = Vec<int>;

using Adjacent = Vec<Vec<bool>>;
using WeightedEdge = Pair<int, int>;
using Graph = Vec<Vec<int64>>;

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
  IO();

  template <typename T> auto ReadT() const;

  template <typename T> auto WriteT() const;

  void nl() const;
  void sp() const;
};

IO::IO() : input(std::cin), output(std::cout) {
  input.tie(nullptr);
  output.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
}

template <typename T> auto IO::ReadT() const {
  return [this]() {
    T v;
    input >> v;
    return v;
  };
}

template <typename T> auto IO::WriteT() const {
  return [this](const T &v) { output << v; };
}

void IO::nl() const { output << std::endl; }

void IO::sp() const { output << ' '; }

struct Edge {
  const int from;
  const int to;
  const int64 weight;
};

int main() {
  const auto io = IO();
  const auto readInt = io.ReadT<int>();
  const auto readInt64 = io.ReadT<int64>();
  const auto writeInt = io.WriteT<int>();
  const auto writeInt64 = io.WriteT<int64>();
  const auto writeString = io.WriteT<std::string>();
  const int64 oo = 7'000'000'000'000'000'000;

  const auto n = readInt();
  const auto m = readInt();
  const auto s = readInt() - 1;
  // list of all edges for ford-bellman
  auto edges = Vec<Edge>();
  // simple adjacency graph
  auto g = Vec<Vec<int>>(n, Vec<int>());
  edges.reserve(m);
  for (const auto _ : Range(m)) {
    edges.push_back(
        {.from = readInt() - 1, .to = readInt() - 1, .weight = readInt64()});
    g[edges.back().from].push_back(edges.back().to);
  }

  auto distances = Vec<int64>(n, oo);
  auto noShortestPath = Vec<bool>(n, false);
  distances[s] = 0;

  // first n - 1 iterations will be finding actual shortest paths
  // next  n - 1 iterations will be finding vertices which are on or can be
  //   reached from a negative-weight cycle
  for (const auto iteration : Range(2 * n)) {
    for (const auto [from, to, weight] : edges) {
      if (distances[from] < oo && distances[to] > distances[from] + weight) {
        // if we can optimize further than max length,
        // then the vertex has shortest path = -oo
        if (iteration > n - 1) {
          noShortestPath[to] = true;
        }
        distances[to] = std::max(-oo, distances[from] + weight);
      }
    }
  }

  auto visited = Vec<bool>(n, false);

  std::function<void(int)> dfs;
  dfs = [&](int v) {
    visited[v] = true;
    for (const auto to : g[v]) {
      if (!visited[to]) {
        dfs(to);
      }
    }
  };

  for (const auto v : Range(n)) {
    if (noShortestPath[v] && !visited[v]) {
      dfs(v);
    }
  }

  for (const auto i : Range(n)) {
    if (distances[i] == oo) {
      writeString("*");
    } else if (visited[i]) {
      writeString("-");
    } else {
      writeInt64(distances[i]);
    }
    io.nl();
  }
}