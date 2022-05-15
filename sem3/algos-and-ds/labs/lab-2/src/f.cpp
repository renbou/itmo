#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using uint64 = uint64_t;
using uint32 = uint32_t;
using int64 = int64_t;
using int32 = int32_t;

template <typename T> using Vec = std::vector<T>;
template <typename T, typename U> using Pair = std::pair<T, U>;
using IntVec = Vec<int>;

using Adjacent = Vec<Vec<bool>>;

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
  const int to;
  const int64 weight;
};
using Graph = Vec<Vec<Edge>>;
const int64 oo = 1'000'000'000'000'000;

int64 shortestDistance(const Graph &g, const int from, const int to) {
  const int n = g.size();
  auto distances = Vec<int64>(n, oo);
  auto queue = std::priority_queue<Pair<int64, int>, Vec<Pair<int64, int>>,
                                   std::greater<Pair<int64, int>>>();

  distances[from] = 0;
  queue.push({0, from});

  while (!queue.empty()) {
    const auto [distanceToV, v] = queue.top();
    queue.pop();
    if (distanceToV != distances[v]) {
      // old pair. distances[v] is best since we picked it, and dist !=
      // distances[v], thus dist > distances[v]
      continue;
    }

    if (v == to) {
      // picked v == to, thus answer is distances[to]
      break;
    }

    for (const auto [to, weight] : g[v]) {
      const auto newDistance = distanceToV + weight;
      if (newDistance < distances[to]) {
        distances[to] = newDistance;
        queue.push({distances[to], to});
      }
    }
  }

  return distances[to];
}

int64 countDistance(const int64 partA, const int64 partB) {
  if (partA == oo || partB == oo) {
    return oo;
  }
  return partA + partB;
}

int main() {
  const auto io = IO();
  const auto readInt = io.ReadT<int>();
  const auto readInt64 = io.ReadT<int64>();
  const auto writeInt = io.WriteT<int>();
  const auto writeInt64 = io.WriteT<int64>();
  const auto writeString = io.WriteT<std::string>();

  const auto n = readInt();
  const auto m = readInt();

  auto g = Graph(n, Vec<Edge>());
  for (const auto _ : Range(m)) {
    const auto u = readInt() - 1;
    const auto v = readInt() - 1;
    const auto w = readInt();
    g[u].push_back({.to = v, .weight = w});
    g[v].push_back({.to = u, .weight = w});
  }

  const auto a = readInt() - 1;
  const auto b = readInt() - 1;
  const auto c = readInt() - 1;

  const auto aToB = shortestDistance(g, a, b);
  const auto aToC = shortestDistance(g, a, c);
  const auto cToA = aToC;
  const auto bToC = shortestDistance(g, b, c);

  const auto distanceOne = countDistance(aToB, bToC);
  const auto distanceTwo = countDistance(bToC, cToA);
  const auto distanceThree = countDistance(cToA, aToB);

  const auto min = std::min({distanceOne, distanceTwo, distanceThree});
  writeInt64(min == oo ? -1 : min);
  io.nl();
}