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

class SuffixArray {
public:
  const int AlphabetSize = 256;
  SuffixArray(const std::string &s) {
    // sort cyclic shifts for str + char lower than all other chars
    // this will in fact sort cyclic substrings
    suffixArray = sortCyclicShifts(s + "#");
    // remove the first entry which of course starts with #
    suffixArray.erase(suffixArray.begin());

    // now build lcp with the build suffix array and string
    lcp = buildLCP(s);
  }

  // sorts cyclic shifts of string s and returns array
  // containing indices of the start of each cyclic shift
  IntVec sortCyclicShifts(const std::string &str) {
    // size of the string, suffix array, etc
    const int n = str.size();
    // internal representation of the suffix array
    IntVec suffixArray(n, 0);
    // equivalence classes built during sort
    IntVec equivalenceClasses(n, 0);
    // array used during counting sort
    IntVec countingSortArray(std::max(AlphabetSize, n), 0);

    // base iteration, sorting cyclic substrings of length 2^0=1
    for (const char &c : str) {
      countingSortArray[c]++;
    }
    for (const int i : Range(1, AlphabetSize)) {
      // give a valid "range" to each character so that the lexicographically
      // larger characters come later
      countingSortArray[i] += countingSortArray[i - 1];
    }
    for (const int i : Range(n)) {
      // knowing the index of the current cyclic substring of length 1 (starting
      // at i) set the value at that position in the suffix array to i
      suffixArray[--countingSortArray[str[i]]] = i;
    }

    // compute equivalence classes for later use
    int equivalenceClass = 0;
    equivalenceClasses[suffixArray[0]] = equivalenceClass;
    for (const int i : Range(1, n)) {
      if (str[suffixArray[i]] != str[suffixArray[i - 1]]) {
        equivalenceClass++;
      }
      equivalenceClasses[suffixArray[i]] = equivalenceClass;
    }

    // now sort cyclic substrings of size 2^(k+1)
    // create temporary suffix array, with incomplete sort
    // and create temporary equivalence class array
    IntVec temporarySuffixArray(n);
    IntVec temporaryEquivalenceClasses(n);
    for (int size = 1; size < n; size <<= 1) {
      // subtract 2^k from start of each cyclic substring, this will give us a
      // suffix array with substrings of size 2^k where only the smallest digit
      // is sorted
      for (const int i : Range(n)) {
        temporarySuffixArray[i] = suffixArray[i] - size;
        if (temporarySuffixArray[i] < 0) {
          temporarySuffixArray[i] += n;
        }
      }

      // counting-sort the big digit
      std::fill(countingSortArray.begin(),
                countingSortArray.begin() + equivalenceClass + 1, 0);
      // extra level of indirection - using the indices from the suffix array
      // is needed for the sort to be stable and actually work
      for (const int i : Range(n)) {
        countingSortArray[equivalenceClasses[temporarySuffixArray[i]]]++;
      }
      for (const int i : Range(1, equivalenceClass + 1)) {
        countingSortArray[i] += countingSortArray[i - 1];
      }
      for (int i = n - 1; i >= 0; i--) {
        suffixArray
            [--countingSortArray[equivalenceClasses[temporarySuffixArray[i]]]] =
                temporarySuffixArray[i];
      }

      // fill the new equivalence classes
      equivalenceClass = 0;
      temporaryEquivalenceClasses[suffixArray[0]] = equivalenceClass;
      for (const int i : Range(1, n)) {
        auto first =
            std::make_pair(equivalenceClasses[suffixArray[i]],
                           equivalenceClasses[(suffixArray[i] + size) % n]);
        auto second =
            std::make_pair(equivalenceClasses[suffixArray[i - 1]],
                           equivalenceClasses[(suffixArray[i - 1] + size) % n]);
        if (first != second) {
          equivalenceClass++;
        }
        temporaryEquivalenceClasses[suffixArray[i]] = equivalenceClass;
      }
      equivalenceClasses.swap(temporaryEquivalenceClasses);
    }

    return suffixArray;
  }

  IntVec buildLCP(const std::string &str) {
    const int n = str.size();
    // reverse array
    IntVec positionInSuffixArray(n, 0);
    IntVec lcp(n - 1, 0);
    for (const int i : Range(n)) {
      positionInSuffixArray[suffixArray[i]] = i;
    }

    int matched = 0;
    // iterate over every substring starting at i
    for (const int i : Range(n)) {
      if (positionInSuffixArray[i] == n - 1) {
        // last item in the suffix array -> no next substring -> no lcp and no
        // match
        matched = 0;
        continue;
      }

      // next substring in suffix array order
      const int j = suffixArray[positionInSuffixArray[i] + 1];
      while (i + matched < n && j + matched < n &&
             str[i + matched] == str[j + matched]) {
        matched++;
      }
      lcp[positionInSuffixArray[i]] = matched;

      // for all next items matched-1 is definitely a match, might increase
      // this is the optimisation which gives O(n)
      if (matched) {
        matched--;
      }
    }

    return lcp;
  }

  IntVec suffixArray;
  IntVec lcp;
};

int main() {
  const auto io = IO(std::cin, std::cout);
  const auto s = io.read<std::string>();
  SuffixArray suffixArray(s);
  for (const int i : suffixArray.suffixArray) {
    io.write(i + 1);
    io.sp();
  }
  io.nl();
  for (const int i : suffixArray.lcp) {
    io.write(i);
    io.sp();
  }
  io.nl();
}