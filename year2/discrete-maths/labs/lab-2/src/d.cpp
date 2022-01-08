#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
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

bool validateAxiomOne(std::vector<std::vector<int>> &setsBySize,
                      std::vector<bool> &setMap) {
  return !setsBySize[0].empty();
}

bool validateAxiomTwo(std::vector<std::vector<int>> &setsBySize,
                      std::vector<bool> &setMap) {
  const int n = setsBySize.size() - 1;
  // check that for each independent set of each size
  // the subsets of size current-1 exist
  for (int setSize = n; setSize >= 0; setSize--) {
    for (const int bitset : setsBySize[setSize]) {
      for (const int bit : Range(n)) {
        const int element = (1 << bit);
        if ((bitset & element) != 0) {
          const int subset = (bitset ^ element);
          if (!setMap[subset]) {
            return false;
          }
        }
      }
    }
  }
  return true;
}

bool validateAxiomThree(std::vector<std::vector<int>> &setsBySize,
                        std::vector<bool> &setMap) {
  const int n = setsBySize.size() - 1;
  for (int AsetSize = 0; AsetSize <= n; AsetSize++) {
    for (int BsetSize = AsetSize + 1; BsetSize <= n; BsetSize++) {
      for (const int bitsetA : setsBySize[AsetSize]) {
        for (const int bitsetB : setsBySize[BsetSize]) {
          // true if there exists an element in B\A which
          // can be added to A and give an independent set
          bool existsElement = false;
          for (const int bit : Range(n)) {
            const int element = (1 << bit);
            // if element is in B (size+x) but not in A (size))
            if (((bitsetB & element) != 0) && ((bitsetA & element) == 0)) {
              const int subset = (bitsetA ^ element);
              if (setMap[subset]) {
                existsElement = true;
                break;
              }
            }
          }
          if (!existsElement) {
            return false;
          }
        }
      }
    }
  }
  return true;
}

int main() {
  freopen("check.in", "r", stdin);
  freopen("check.out", "w", stdout);

  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
  const auto io = IO(std::cin, std::cout);

  const auto n = io.read<int>();
  const auto m = io.read<int>();

  // auto setsBySize = std::vector<std::vector<std::unordered_set<int>>>(n + 1);
  auto setsBySize = std::vector<std::vector<int>>(n + 1);
  auto setMap = std::vector<bool>(1 << n);
  for (const int _ : Range(m)) {
    const auto setSize = io.read<int>();
    // auto set = setsBySize[setSize].emplace_back(setSize);
    int bitset = 0;
    for (const int __ : Range(setSize)) {
      const int element = io.read<int>() - 1;
      // set.emplace(element);
      bitset |= (1 << element);
    }
    setMap[bitset] = true;
    setsBySize[setSize].emplace_back(bitset);
  }

  const std::vector axioms = {validateAxiomOne, validateAxiomTwo,
                              validateAxiomThree};

  if (std::all_of(axioms.begin(), axioms.end(), [&](const auto &axiom) {
        return axiom(setsBySize, setMap);
      })) {
    io.write("YES");
  } else {
    io.write("NO");
  }
  io.nl();
}