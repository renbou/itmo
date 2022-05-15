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

struct Element {
  int id;
  int64 weight;
};

// if cycle is contained in bitset, then the bitset is NOT independent
// so if no cycle is contained in the bitset, then it IS independent
bool isIndependent(int bitset, std::vector<int> &cycles) {
  return std::all_of(cycles.begin(), cycles.end(), [&](const auto &cycle) {
    return (bitset & cycle) != cycle;
  });
}

int main() {
  freopen("cycles.in", "r", stdin);
  freopen("cycles.out", "w", stdout);

  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
  const auto io = IO(std::cin, std::cout);

  const auto n = io.read<int>();
  const auto m = io.read<int>();

  // read the weights of ground set elements
  std::vector<Element> ground(n);
  for (const int i : Range(n)) {
    io.read(ground[i].weight);
    ground[i].id = i;
  }

  // read all cycles, representing them as cycles
  std::vector<int> cycles(m);
  for (const int i : Range(m)) {
    const auto cycleSize = io.read<int>();
    int bitset = 0;
    for (const int _ : Range(cycleSize)) {
      const int element = io.read<int>() - 1;
      bitset |= (1 << element);
    }
    cycles.at(i) = bitset;
  }

  // for Rado-Edmonds algorithm sort weights in decreasing order
  std::sort(
      ground.begin(), ground.end(),
      [](const Element &a, const Element &b) { return a.weight > b.weight; });

  int base = 0;
  int64 baseWeight = 0;
  for (const auto &element : ground) {
    int newbase = base | (1 << element.id);
    if (isIndependent(newbase, cycles)) {
      base = newbase;
      baseWeight += element.weight;
    }
  }

  io.write(baseWeight);
  io.nl();
}