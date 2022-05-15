#include <algorithm>
#include <array>
#include <cstddef>
#include <ios>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <stdexcept>
#include <unordered_set>
#include <vector>

using uint64 = uint64_t;
using uint32 = uint32_t;
using int64 = int64_t;
using int32 = int32_t;

#define var auto
#define val const auto

using std::endl, std::cin, std::cout;

template <typename T> using List = std::list<T>;
template <typename T> using Vec = std::vector<T>;
template <typename T> using Queue = std::queue<T>;

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

  Range(const int from, const int to) : from(from), to(to) {}

  iterator begin() { return iterator(from, to, from); }
  iterator end() { return iterator(from, to, to); }
};

class IO {
  std::istream &input;
  std::ostream &output;

public:
  IO();

  template <typename T> auto ReadT();

  template <typename T> auto WriteT();

  void nl();
  void sp();
};

IO::IO() : input(std::cin), output(std::cout) {
  input.tie(nullptr);
  output.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
}

template <typename T> auto IO::ReadT() {
  return [this]() {
    T v;
    input >> v;
    return v;
  };
}

template <typename T> auto IO::WriteT() {
  return [this](const T &v) { output << v; };
}

void IO::nl() {
  output << std::endl;
}

void IO::sp() {
  output << ' ';
}

int main() {
  IO io;
  val readInt = io.ReadT<int>();
  val writeInt = io.WriteT<int>();

  val n{readInt()};
  val m{readInt()};
  var g = Graph(n);
  for (val _ : Range(0, m)) {
    int u{readInt() - 1};
    int v{readInt() - 1};
    g[u].push_back(v);
    g[v].push_back(u);
  }

  var maxDeg = -1;
  var maxDegV = -1;
  for (var v : Range(0, n)) {
    if (int(g[v].size()) > maxDeg) {
      maxDeg = g[v].size();
      maxDegV = v;
    }
  }
  // If max deg is even then increase it to an odd number
  if (maxDeg % 2 == 0) {
    ++maxDeg;
  }

  // Run dfs starting at node with largest deg
  var colors = Vec<int>(n, -1);
  var visited = Vec<bool>(n, false);
  var neighbourColors = Vec<bool>(maxDeg, false);

  var q = Queue<int>();
  q.push(maxDegV);
  while (not q.empty()) {
    val u = q.front();
    q.pop();
    visited[u] = true;

    // Pick color which isn't used by neighbours
    for (val v : g[u]) {
      if (colors[v] != -1) {
        neighbourColors[colors[v]] = true;
      }
    }
    var color = -1;
    for (val i : Range(0, maxDeg)) {
      if (not neighbourColors[i]) {
        color = i;
        break;
      }
    }
    std::fill(neighbourColors.begin(), neighbourColors.end(), false);

    colors[u] = color;
    for (val v : g[u]) {
      if (!visited[v]) {
        q.push(v);
      }
    }
  }

  writeInt(maxDeg);
  io.nl();
  for (val c : colors) {
    writeInt(c + 1);
    io.sp();
  }
  io.nl();

  return 0;
}