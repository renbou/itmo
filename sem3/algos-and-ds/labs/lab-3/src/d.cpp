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

class PolynomialHash {
public:
  static const __int128 p = 59ll;
  static const __int128 m = 1000000000000037ll;
  static std::vector<int64> cachedPowers;

  PolynomialHash(const int64 hash, const int startIndex, const int length)
      : hash(hash), startIndex(startIndex), powerIndex(startIndex + length) {}

  PolynomialHash() : PolynomialHash(0, 0, 0) {}

  // precalculate specified amount of powers
  void preCalculatePowers(int n) const {
    const int previousN = cachedPowers.size();
    if (n < previousN) {
      return;
    }

    cachedPowers.resize(n + 1);
    for (auto i : Range(previousN, n + 1)) {
      cachedPowers[i] = int64((__int128(cachedPowers[i - 1]) * p) % m);
    }
  }

  // get specified power of p, this also fills the cache
  __int128 getPowerOfP(int power) const {
    if (power < cachedPowers.size()) {
      return cachedPowers.at(power);
    }
    preCalculatePowers(power);
    return __int128(getPowerOfP(power));
  }

  // get index of char in alphabet starting from 1
  static int charIndexInAlphabet(char c) {
    if (c >= 'a' && c <= 'z') {
      return c - 'a' + 1;
    }
    if (c >= 'A' && c <= 'Z') {
      return (c - 'A' + 1) + 26;
    }
    throw new std::runtime_error("invalid alphabet");
  }

  void update(const char c) {
    hash = int64((__int128(hash) +
                  __int128(charIndexInAlphabet(c)) * getPowerOfP(powerIndex)) %
                 m);
    powerIndex++;
  }

  void update(const std::string &str) {
    for (const auto c : str) {
      update(c);
    }
  }

  int64 sum() const { return hash; }

  // compare hashes. don't compare lengths (powerIndex - startIndex) since it's
  // just an extra useless if
  bool operator==(const PolynomialHash &other) const {
    __int128 hasha = __int128(this->hash);
    __int128 hashb = __int128(other.hash);
    if (this->startIndex <= other.startIndex) {
      hasha = (hasha * getPowerOfP(other.startIndex - this->startIndex)) % m;
    } else {
      hashb = (hashb * getPowerOfP(this->startIndex - other.startIndex)) % m;
    }

    return int64(hasha) == int64(hashb);
  }

  bool operator!=(const PolynomialHash &other) const {
    return !this->operator==(other);
  }

private:
  // the current calculated sum
  int64 hash;
  // the original power of the hash, in case this was extracted from the center
  // of a string. ex: hash of s[2..5] has base power of 2. this is actually
  // needed only for comparison
  const int startIndex;
  // current power index
  int powerIndex;
};
std::vector<int64> PolynomialHash::cachedPowers = {1};

class HashedString {
public:
  HashedString(const std::string &str) : str(str), prefixHashes(str.size()) {
    PolynomialHash hash{};
    for (const int i : Range(str.size())) {
      hash.update(str[i]);
      prefixHashes[i] = hash.sum();
    }
  }

  // get hash of substring s[i..j]
  PolynomialHash substringHash(const int i, const int j) const {
    if (j < i) {
      throw new std::runtime_error("cannot take substring hash with j < i");
    }

    __int128 hashValue = __int128(prefixHashes.at(j));
    if (i > 0) {
      hashValue =
          (hashValue - __int128(prefixHashes.at(i - 1)) + PolynomialHash::m) %
          PolynomialHash::m;
    }

    return PolynomialHash(hashValue, i, j - i + 1);
  }

private:
  const std::string str;
  std::vector<int64> prefixHashes;
};

int main() {
  const auto io = IO();
  const auto readInt = io.ReadT<int>();
  const auto writeInt = io.WriteT<int>();
  const auto readString = io.ReadT<std::string>();
  const auto writeString = io.WriteT<std::string>();

  const auto s = readString();
  const auto sHashed = HashedString(s);
  const auto sHash = sHashed.substringHash(0, s.size() - 1);

  const auto t = readString();
  const auto tHashed = HashedString(t);

  auto occurrences = IntVec();
  for (int i = 0; i + s.size() - 1 < t.size(); i++) {
    if (sHash == tHashed.substringHash(i, i + s.size() - 1)) {
      occurrences.push_back(i);
    }
  }

  writeInt(occurrences.size());
  io.nl();
  for (const auto o : occurrences) {
    writeInt(o + 1);
    io.sp();
  }
  io.nl();
}