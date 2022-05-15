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
  const auto writeInt = io.WriteT<int>();
  const auto writeString = io.WriteT<std::string>();
  const int64 oo = 1'000'000'000;

  const auto n = readInt();
  const auto m = readInt();
  const auto k = readInt();
  const auto s = readInt() - 1;
  auto edges = Vec<Edge>();
  edges.reserve(m);
  for (const auto _ : Range(m)) {
    edges.push_back(
        {.from = readInt() - 1, .to = readInt() - 1, .weight = readInt()});
  }

  auto distances = Vec<Vec<int64>>(k + 1, Vec<int64>(n, oo));
  distances[0][s] = 0;
  // relax k times
  for (const auto i : Range(1, k + 1)) {
    for (const auto [from, to, weight] : edges) {
      if (distances[i - 1][from] < oo &&
          distances[i][to] >= distances[i - 1][from] + weight) {
        distances[i][to] = distances[i - 1][from] + weight;
      }
    }
  }

  for (const auto d : distances[k]) {
    writeInt(d == oo ? -1 : d);
    io.sp();
  }
  io.nl();
}