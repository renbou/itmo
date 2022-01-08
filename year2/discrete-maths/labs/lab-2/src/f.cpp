#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <iterator>
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

class DSU {
public:
  DSU(int n) : parent(n), size(n) {
    for (const int i : Range(n)) {
      parent[i] = i;
      size[i] = 1;
    }
  }

  int findRoot(const int v) {
    if (v == parent[v]) {
      return v;
    }
    return parent[v] = findRoot(parent[v]);
  }

  void unite(const int a, const int b) {
    int rootA = findRoot(a);
    int rootB = findRoot(b);
    if (rootA != rootB) {
      if (size[rootA] < size[rootB]) {
        std::swap(rootA, rootB);
      }
      parent[rootB] = rootA;
      size[rootA] += size[rootB];
    }
  }

  bool areInSameSet(const int a, const int b) {
    return findRoot(a) == findRoot(b);
  }

  bool areInDifferentSets(const int a, const int b) {
    return findRoot(a) != findRoot(b);
  }

private:
  IntVec parent;
  IntVec size;
};

const int MAX_COLOR = 100;

// representation of a single edge in the exchange graph, undirected
// chosen is the element already in the independent set
// free is the element for which the chosen one can be exchanged for
struct ExchangeEdge {
  ExchangeEdge() = default;

  ExchangeEdge(const int chosen, const int free) : chosen(chosen), free(free) {}

  int chosen;
  int free;
};

// representation of a single state during matroid intersection
// the elements currently chosen as independent, and the rest
struct IntersectionState {
  IntVec chosen;
  IntVec others;
};

class ColourMatroid {
public:
  ColourMatroid(const IntVec &elementColour) : elementColour(elementColour) {}

  // state of the colour matroid during a single
  // matroid intersection state
  class ColourMatroidState {
  public:
    ColourMatroidState(const ColourMatroid &matroid,
                       const IntersectionState &state)
        : matroid(matroid), state(state), usedColour(MAX_COLOR + 1, false) {
      for (const int chosenElement : state.chosen) {
        useElementColour(chosenElement);
      }
    }

    // returns elements which can increase the size of
    // chosen independent set by adding one of them
    IntVec getAugmentingElements() const {
      IntVec augmenting;
      for (const int freeElement : state.others) {
        if (!elementColourUsed(freeElement)) {
          augmenting.emplace_back(freeElement);
        }
      }
      return augmenting;
    }

    // returns elements which can be exchanged, for the exchange graph
    std::vector<ExchangeEdge> getExchangeableElements() const {
      std::vector<ExchangeEdge> exchangeable;
      for (const int freeElement : state.others) {
        for (const int chosenElement : state.chosen) {
          // if element colour isn't used at all or if the colour is the same,
          // then of course we can exchange these two elements
          if (!elementColourUsed(freeElement) ||
              matroid.colourOf(freeElement) ==
                  matroid.colourOf(chosenElement)) {
            exchangeable.emplace_back(chosenElement, freeElement);
          }
        }
      }
      return exchangeable;
    }

  private:
    void useElementColour(int element) {
      usedColour.at(matroid.colourOf(element)) = true;
    }

    bool elementColourUsed(int element) const {
      return usedColour.at(matroid.colourOf(element));
    }

    const ColourMatroid &matroid;
    const IntersectionState &state;
    std::vector<bool> usedColour;
  };

  ColourMatroidState getMatroidState(const IntersectionState &state) const {
    return ColourMatroidState(*this, state);
  }

  int colourOf(int element) const { return elementColour.at(element); }

private:
  // characterization of each element in the ground set
  const IntVec &elementColour;
};

class GraphicMatroid {
public:
  struct Edge {
    int u;
    int v;
  };

  GraphicMatroid(const int numVertices, const std::vector<Edge> &edges)
      : numVertices(numVertices), edges(edges) {}

  // state of the graphic matroid during a single
  // matroid intersection state
  class GraphicMatroidState {
  public:
    GraphicMatroidState(const GraphicMatroid &matroid,
                        const IntersectionState &state)
        : matroid(matroid), state(state) {}

    // returns elements which can increase the size of
    // chosen independent set by adding one of them
    IntVec getAugmentingElements() const {
      // merge all chosen edge's vertices into single set,
      // then check which free elements lie in different sets,
      // meaning that adding one of them won't ruin independence
      DSU dsu(matroid.getNumVertices());
      for (const int chosenElement : state.chosen) {
        const auto &edge = matroid.getEdge(chosenElement);
        dsu.unite(edge.u, edge.v);
      }

      IntVec augmenting;
      for (const int freeElement : state.others) {
        const auto &edge = matroid.getEdge(freeElement);
        if (dsu.areInDifferentSets(edge.u, edge.v)) {
          augmenting.emplace_back(freeElement);
        }
      }
      return augmenting;
    }

    // returns elements which can be exchanged, for the exchange graph
    std::vector<ExchangeEdge> getExchangeableElements() const {
      std::vector<ExchangeEdge> exchangeable;

      // for each chosen element, create a dsu with a large set
      // consisting of vertices from all OTHER chosen elements
      // then check which free element we can add
      for (const int chosenElement : state.chosen) {
        // create single set in dsu without vertices from current chosen element
        DSU dsu(matroid.getNumVertices());
        for (const int differentChosenElement : state.chosen) {
          if (differentChosenElement != chosenElement) {
            const auto &edge = matroid.getEdge(differentChosenElement);
            dsu.unite(edge.u, edge.v);
          }
        }

        // check which free elements we can add now, instead of current chosen
        // element
        for (const int freeElement : state.others) {
          const auto &edge = matroid.getEdge(freeElement);
          if (dsu.areInDifferentSets(edge.u, edge.v)) {
            exchangeable.emplace_back(chosenElement, freeElement);
          }
        }
      }
      return exchangeable;
    }

  private:
    const GraphicMatroid &matroid;
    const IntersectionState &state;
  };

  GraphicMatroidState getMatroidState(const IntersectionState &state) const {
    return GraphicMatroidState(*this, state);
  }

  int getNumVertices() const { return numVertices; }

  const Edge &getEdge(int id) const { return edges.at(id); }

private:
  const int numVertices;
  const std::vector<Edge> &edges;
};

IntVec findAugmentingPath(const Graph &exchangeGraph, const IntVec &sources,
                          const IntVec &sinks) {
  const int m = exchangeGraph.size();

  // build table to quickly check if element is a sink
  std::vector<bool> isSink(m, false);
  for (const int sink : sinks) {
    isSink.at(sink) = true;
  }

  // initialize bfs, originally we start from all source elements
  std::vector<bool> used(m, false);
  std::queue<int> q;
  IntVec parent(m, -1);
  for (const int source : sources) {
    used.at(source) = true;
    q.push(source);
  }

  // launch bfs to find shortest augmenting path
  int sink = -1;
  while (!q.empty()) {
    int v = q.front();
    q.pop();

    // if we found a sink, we can immediately use it
    if (isSink.at(v)) {
      sink = v;
      break;
    }

    // if we are currently in the chosen half, this will get all free elements
    // for which the chosen element can be exchanged for
    // otherwise this will get all chosen elements for which this free element
    // can be exchanged
    for (const int to : exchangeGraph.at(v)) {
      if (!used.at(to)) {
        used.at(to) = true;
        q.push(to);
        parent.at(to) = v;
      }
    }
  }

  // get path using the parent array
  IntVec path;
  for (int cur = sink; cur != -1; cur = parent.at(cur)) {
    path.emplace_back(cur);
  }
  return path;
}

bool tryAugment(const ColourMatroid &colourMatroid,
                const GraphicMatroid &graphicMatroid,
                std::vector<bool> &isChosen) {
  const int m = isChosen.size();

  // initialize the state for this augmentation stage
  IntersectionState state;
  for (const int i : Range(m)) {
    if (isChosen.at(i)) {
      state.chosen.emplace_back(i);
    } else {
      state.others.emplace_back(i);
    }
  }

  const auto colourMatroidState = colourMatroid.getMatroidState(state);
  const auto graphicMatroidState = graphicMatroid.getMatroidState(state);

  // build the exchange graph using current matroid states
  Graph exchangeGraph(m);
  for (const auto &edge : colourMatroidState.getExchangeableElements()) {
    exchangeGraph.at(edge.chosen).emplace_back(edge.free);
  }
  for (const auto &edge : graphicMatroidState.getExchangeableElements()) {
    exchangeGraph.at(edge.free).emplace_back(edge.chosen);
  }

  const auto sources = colourMatroidState.getAugmentingElements();
  const auto sinks = graphicMatroidState.getAugmentingElements();

  const auto augmentingPath = findAugmentingPath(exchangeGraph, sources, sinks);
  if (augmentingPath.empty()) {
    // nothing found, augmentation failed
    return false;
  }

  // get S^P, the new independent set with size+1
  for (const int v : augmentingPath) {
    isChosen.at(v) = !isChosen.at(v);
  }
  return true;
}

int main() {
  freopen("rainbow.in", "r", stdin);
  freopen("rainbow.out", "w", stdout);

  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
  const auto io = IO(std::cin, std::cout);

  const auto n = io.read<int>();
  const auto m = io.read<int>();

  std::vector<GraphicMatroid::Edge> edges(m);
  IntVec colours(m);

  for (const int i : Range(m)) {
    edges.at(i) = {io.read<int>() - 1, io.read<int>() - 1};
    io.read(colours.at(i));
  }

  ColourMatroid colourMatroid(colours);
  GraphicMatroid graphicMatroid(n, edges);

  // isChosen[i] is true if element with index (id) i
  // is currently in the independent set S
  std::vector<bool> isChosen(m, false);

  // augment and increase the independent set while we can
  bool augmented = true;
  while (augmented) {
    augmented = tryAugment(colourMatroid, graphicMatroid, isChosen);
  };

  // finally get the chosen elements
  IntVec chosen;
  for (const int i : Range(m)) {
    if (isChosen.at(i)) {
      chosen.emplace_back(i + 1);
    }
  }

  io.write(chosen.size());
  io.nl();
  for (const int el : chosen) {
    io.write(el);
    io.sp();
  }
  io.nl();
}