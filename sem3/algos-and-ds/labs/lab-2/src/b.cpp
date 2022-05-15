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
using Graph = Vec<Vec<WeightedEdge>>;

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

int main() {
  const auto io = IO();
  const auto readInt = io.ReadT<int>();
  const auto writeInt = io.WriteT<int>();

  const auto n = readInt();
  const auto m = readInt();
  auto g = Graph(n, Vec<WeightedEdge>());
  for (const auto i : Range(m)) {
    const auto u = readInt() - 1;
    const auto v = readInt() - 1;
    const auto w = readInt();
    g[u].push_back(WeightedEdge(v, w));
    g[v].push_back(WeightedEdge(u, w));
  }

  const int64 oo = 1'000'000'000'000;
  auto distances = Vec<int64>(n, oo);
  distances[0] = 0;

  auto queue = std::set<Pair<int64, int>>();
  queue.insert({0, 0});

  while (!queue.empty()) {
    const auto [dist, v] = *queue.begin();
    queue.erase(queue.begin());

    for (const auto &edge : g[v]) {
      const auto newDistance = dist + edge.second;
      if (newDistance < distances[edge.first]) {
        queue.erase({distances[edge.first], edge.first});
        distances[edge.first] = newDistance;
        queue.insert({distances[edge.first], edge.first});
      }
    }
  }

  for (const auto distance : distances) {
    writeInt(distance);
    io.sp();
  }
  io.nl();
}