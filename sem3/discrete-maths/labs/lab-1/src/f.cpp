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
  var pruferCode = Vec<int>(n - 2);
  var g = Graph(n);

  for (val i : Range(0, n - 2)) {
    pruferCode[i] = readInt() - 1;
  }

  // In prufer code each vertex is encountered deg-1 times
  var deg = Vec<int>(n, 1);
  for (val u: pruferCode) {
    deg[u]++;
  }
  // After we know the degrees we can find out the leaves just like before
  var leaves = std::priority_queue<int, Vec<int>, std::greater<>>();
  for (val u : Range(0, n)) {
    if (deg[u] == 1) {
      leaves.push(u);
    }
  }

  // Since on each iteration we know the leaf we are working on,
  // we can simply add the edge to the graph
  for (val v : pruferCode) {
    val u = leaves.top();
    leaves.pop();

    g[u].push_back(v);
    if (--deg[v] == 1) {
      leaves.push(v);
    }
  }
  g[leaves.top()].push_back(n - 1);

  for (val u: Range(0, n)) {
    for (val v: g[u]) {
      writeInt(u + 1); io.sp(); writeInt(v + 1);
    }
    io.nl();
  }

  return 0;
}