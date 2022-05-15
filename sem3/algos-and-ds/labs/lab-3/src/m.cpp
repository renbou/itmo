#include <algorithm>
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

class PolynomialHash {
public:
  static const __int128 p = 59ll;
  static const __int128 pInv = 966101694915290ll;
  static const __int128 m = 1000000000000037ll;
  static std::vector<int64> cachedPowersOfP;
  static std::vector<int64> cachedPowersOfInvP;

  PolynomialHash(const int64 hash, const int length)
      : hash(hash), powerIndex(length) {}

  PolynomialHash() : PolynomialHash(0, 0) {}

  // precalculate specified amount of powers
  static void preCalculatePowers(int n) {
    const int previousN = cachedPowersOfP.size();
    if (n < previousN) {
      return;
    }

    cachedPowersOfP.resize(n + 1);
    cachedPowersOfInvP.resize(n + 1);
    for (auto i : Range(previousN, n + 1)) {
      cachedPowersOfP[i] = int64((__int128(cachedPowersOfP[i - 1]) * p) % m);
      cachedPowersOfInvP[i] =
          int64((__int128(cachedPowersOfInvP[i - 1]) * pInv) % m);
    }
  }

  // get specified power of p, this also fills the cache
  static __int128 getPowerOfP(int power) {
    if (power < cachedPowersOfP.size()) {
      return cachedPowersOfP.at(power);
    }
    preCalculatePowers(power);
    return __int128(getPowerOfP(power));
  }

  // get specified power of inverted p, this also fills the cache
  static __int128 getPowerOfInvP(int power) {
    if (power < cachedPowersOfInvP.size()) {
      return cachedPowersOfInvP.at(power);
    }
    preCalculatePowers(power);
    return __int128(getPowerOfInvP(power));
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

  bool operator==(const PolynomialHash &other) const {
    return this->hash == other.hash;
  }

  bool operator!=(const PolynomialHash &other) const {
    return !this->operator==(other);
  }

  bool operator<(const PolynomialHash &other) const {
    return this->hash < other.hash;
  }

private:
  // the current calculated sum
  int64 hash;
  // current power index
  int powerIndex;
};
std::vector<int64> PolynomialHash::cachedPowersOfP = {1};
std::vector<int64> PolynomialHash::cachedPowersOfInvP = {1};

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
    hashValue =
        hashValue * PolynomialHash::getPowerOfInvP(i) % PolynomialHash::m;
    return PolynomialHash(hashValue, j - i + 1);
  }

private:
  const std::string str;
  std::vector<int64> prefixHashes;
};

class SolveA {
public:
  static constexpr auto cmp = [](const std::pair<int, PolynomialHash> &a,
                                 const std::pair<int, PolynomialHash> &b) {
    return a.second < b.second;
  };

  SolveA(std::vector<std::string> &initStrings)
      : strings(initStrings), n(initStrings.size()), substringHashes(n) {
    // place the smallest string first, since it has the least amount of
    // substrings which we will be searching for in other strings
    int minLength = strings[0].size();
    int minLengthPos = 0;
    for (const auto i : Range(1, n)) {
      if (strings[i].size() < minLength) {
        minLength = strings[i].size();
        minLengthPos = i;
      }
    }
    std::swap(strings[0], strings[minLengthPos]);

    // hash all of the strings once
    hashedStrings.reserve(n);
    for (const auto &s : strings) {
      hashedStrings.push_back(HashedString(s));
    }
  }

  // prepare hashes of all substrings of length for each string
  // this extracts the hashes and sorts them for faster search later
  void prepareSubstringHashes(int length) {
    for (const auto i : Range(n)) {
      const auto &s = strings.at(i);
      const auto &hs = hashedStrings.at(i);
      auto &currentSubstrHashes = substringHashes.at(i);

      currentSubstrHashes.resize(s.length() - length + 1);
      for (int start = 0; start + length - 1 < s.length(); start++) {
        currentSubstrHashes.at(start) =
            std::make_pair(start, hs.substringHash(start, start + length - 1));
      }

      std::sort(currentSubstrHashes.begin(), currentSubstrHashes.end(), cmp);
    }
  }

  // check if a hash is shared by all strings, meaning that
  // the substring which gave this hash exists in every string
  // hash must be from strings[0]
  bool isSharedSubstring(const std::pair<int, PolynomialHash> &hash) {
    for (const auto i : Range(1, n)) {
      const auto &currentSubstrHashes = substringHashes.at(i);
      const auto found = std::lower_bound(currentSubstrHashes.begin(),
                                          currentSubstrHashes.end(), hash, cmp);
      if (found->second != hash.second) {
        return false;
      }
    }
    return true;
  }

  // try to find a shared substring of given length. returns true if found,
  // false otherwise
  bool findSharedSubstring(int length) {
    prepareSubstringHashes(length);
    for (const auto &substrHash : substringHashes.at(0)) {
      if (isSharedSubstring(substrHash)) {
        startOfLCS = substrHash.first;
        lengthOfLCS = length;
        return true;
      }
    }
    return false;
  }

  std::string longestCommonSubstring() {
    int l = 0;
    int r = strings.at(0).size();
    while (l <= r) {
      int mid = l + (r - l) / 2;
      if (findSharedSubstring(mid)) {
        // shared substring of such length exists, thus we can move l to mid
        l = mid + 1;
      } else {
        // nope, move r to mid-1 since no shared substr of length mid exists
        r = mid - 1;
      }
    }
    return strings.at(0).substr(startOfLCS, lengthOfLCS);
  }

private:
  int n;
  // start and length of largest common substring, inside of strings[0]
  int startOfLCS = 0;
  int lengthOfLCS = 0;
  // actual strings where LCS is to be found
  std::vector<std::string> strings;
  // hashes of the strings, calculated only once
  std::vector<HashedString> hashedStrings;
  std::vector<std::vector<std::pair<int, PolynomialHash>>> substringHashes;
};

int main() {
  const auto io = IO();

  auto strings = std::vector<std::string>(2);
  for (auto &s : strings) {
    io.read(s);
  }

  auto solveA = SolveA(strings);
  auto answer = solveA.longestCommonSubstring();
  io.write(answer);
  io.nl();
}