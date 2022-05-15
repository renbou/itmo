#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

using uint64 = uint64_t;
using uint32 = uint32_t;
using int64 = int64_t;
using int32 = int32_t;

template <typename T> using Queue = std::queue<T>;
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

using Graph = Vec<Vec<int>>;

int main() {
  const auto io = IO();
  const auto readInt = io.ReadT<int>();
  const auto writeInt = io.WriteT<int>();
  const auto writeString = io.WriteT<std::string>();

  int n;
  while (std::cin >> n) {
    const auto m = readInt();
    auto g = Graph(n, Vec<int>());
    auto revG = Graph(n, Vec<int>());
    for (const auto _ : Range(m)) {
      const auto u = readInt() - 1;
      const auto v = readInt() - 1;
      g[u].push_back(v);
      revG[v].push_back(u);
    }

    const int8_t FIRST_WINS = 1;
    const int8_t SECOND_WINS = 0;
    const int8_t NOONE_WINS = -1;

    auto outDegree = Vec<int>(n);
    auto queue = Queue<int>();
    auto vertexWinState = Vec<int8_t>(n, NOONE_WINS);
    for (const auto v : Range(n)) {
      outDegree[v] = g[v].size();
      if ((outDegree[v] = g[v].size()) == 0) {
        queue.push(v);
        vertexWinState[v] = SECOND_WINS;
      }
    }

    while (!queue.empty()) {
      const auto to = queue.front();
      queue.pop();
      for (const auto from : revG[to]) {
        if (vertexWinState[to] == SECOND_WINS &&
            vertexWinState[from] == NOONE_WINS) {
          vertexWinState[from] = FIRST_WINS;
          queue.push(from);
        } else if (vertexWinState[to] == FIRST_WINS && --outDegree[from] == 0) {
          vertexWinState[from] = SECOND_WINS;
          queue.push(from);
        }
      }
    }

    for (const auto result : vertexWinState) {
      switch (result) {
      case FIRST_WINS:
        writeString("FIRST");
        break;
      case SECOND_WINS:
        writeString("SECOND");
        break;
      default:
        writeString("DRAW");
      }
      io.nl();
    }
    io.nl();
  }
}