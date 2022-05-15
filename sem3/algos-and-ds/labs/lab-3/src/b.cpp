#include <iostream>
#include <stdexcept>
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
  IO();

  template <typename T> auto ReadT() const;
  template <typename T> T read() const;
  template <typename T> void read(T &v) const;

  template <typename T> auto WriteT() const;
  template <typename T> void write(const T &v) const;

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

template <typename T> T IO::read() const {
  T v;
  input >> v;
  return v;
}

template <typename T> void IO::read(T &v) const { input >> v; }

template <typename T> auto IO::WriteT() const {
  return [this](const T &v) { output << v; };
}

template <typename T> void IO::write(const T &v) const { output << v; }

void IO::nl() const { output << std::endl; }

void IO::sp() const { output << ' '; }

int main() {
  const auto io = IO();

  const auto s = io.read<std::string>();
  const int n = s.size();
  auto p = std::vector<int>(n);

  p[0] = 0;
  int j = 0;
  for (int i = 1; i < n; i++) {
    while (j > 0 && s[i] != s[j]) {
      j = p[j - 1];
    }
    if (s[i] == s[j]) {
      j++;
    }
    p[i] = j;
  }

  for (const auto &x : p) {
    io.write(x);
    io.sp();
  }
  io.nl();
}