#include <algorithm>
#include <array>
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

class AhoCorasick {
public:
  static const int AlphabetSize = 26;

  class Vertex {
  public:
    Vertex(int p, char pch) : parent(p), pChar(pch), terminal(false), link(-1) {
      stringIndices.reserve(64);
      transitions.fill(-1);
    }

    // automaton transitions
    std::array<int, AlphabetSize> transitions;
    // matching string indices when this is a terminal vertex
    std::vector<int> stringIndices;
    // true if this vertex is terminal, meaning some string ends here
    bool terminal;
    // index of parent vertex
    int parent;
    // which char was used for transition from parent
    char pChar;
    // index of link vertex
    int link;
  };

  AhoCorasick(const std::vector<std::string> &strings)
      : strings(strings), vertices(1, Vertex(-1, '#')) {
    vertices.reserve(100000);
    for (const int i : Range(this->strings.size())) {
      this->addStringToTrie(this->strings[i], i);
    }
  }

  // returns index of char in alphabet
  int charToIndex(char c) { return c - 'a'; }

  // go returns index of vertex where we get after
  // transitioning from v with c
  int go(int v, char ch) {
    int c = charToIndex(ch);
    if (vertices[v].transitions[c] == -1) {
      // if we are at root, then we can only transition to ourselves
      // otherwise we can get the longest existing suffix (memoized link)
      // and transition from it using c
      vertices[v].transitions[c] = (v == 0 ? 0 : go(getLink(v), ch));
    }
    return vertices[v].transitions[c];
  }

  // getLink returns link for vertex v with memoization
  int getLink(int v) {
    if (vertices[v].link == -1) {
      if (v == 0 || vertices[v].parent == 0) {
        // first and second vertices have no valid suffix,
        // thus their link is 0 (empty string)
        vertices[v].link = 0;
      } else {
        vertices[v].link = go(getLink(vertices[v].parent), vertices[v].pChar);
      }
    }
    return vertices[v].link;
  }

  const Vertex &getVertex(int v) { return vertices[v]; }

  std::size_t size() const { return vertices.size(); }

private:
  void addStringToTrie(const std::string &str, int id) {
    int v = 0;
    // add string char-by-char to trie, this will add new vertices if needed
    for (const char ch : str) {
      int c = charToIndex(ch);
      if (vertices[v].transitions[c] == -1) {
        vertices[v].transitions[c] = vertices.size();
        vertices.emplace_back(v, ch);
      }
      v = vertices[v].transitions[c];
    }
    // set last transitioned to vertex as terminal
    vertices[v].terminal = true;
    vertices[v].stringIndices.emplace_back(id);
  }

  const std::vector<std::string> &strings;
  std::vector<Vertex> vertices;
};

void dfsLinks(Graph &linkGraph, std::vector<int64> &counts, int v) {
  for (const int to : linkGraph[v]) {
    dfsLinks(linkGraph, counts, to);
    counts[v] += counts[to];
  }
}

int main() {
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
  const auto io = IO(std::cin, std::cout);

  const auto n = io.read<int>();
  auto strings = std::vector<std::string>(n);
  for (auto &s : strings) {
    io.read(s);
  }
  const auto text = io.read<std::string>();

  // build automaton using aho corasick
  // (actually only build the trie, automaton will be built lazily)
  AhoCorasick ahoCorasick(strings);

  // now traverse the automaton using text
  // and keep count of how many times we've visited each vertex
  std::vector<int64> counts(ahoCorasick.size(), 0);
  int v = 0;
  for (const char c : text) {
    v = ahoCorasick.go(v, c);
    counts[v]++;
  }

  // now build reversed link graph (v->u if u's link is v)
  // so that we know which vertices' counts are to be added
  // to v's count (g[v]), since if they are counted, then their
  // suffix==link is also counted
  Graph linkGraph(ahoCorasick.size());
  for (const int i : Range(1, ahoCorasick.size())) {
    linkGraph[ahoCorasick.getLink(i)].emplace_back(i);
  }
  // now sum up counts starting from the root
  dfsLinks(linkGraph, counts, 0);

  // add matched ids to set
  std::vector<int64> stringCounts(n, 0);
  for (const int i : Range(ahoCorasick.size())) {
    const auto &vertex = ahoCorasick.getVertex(i);
    if (counts[i] && vertex.terminal) {
      for (const int matchIndex : vertex.stringIndices) {
        stringCounts[matchIndex] += counts[i];
      }
    }
  }

  for (const int i : Range(n)) {
    io.write(stringCounts[i]);
    io.nl();
  }
}