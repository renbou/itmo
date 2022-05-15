#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <type_traits>
#include <unordered_set>
#include <vector>

namespace cppren {

namespace type_traits {
template <class T> class remove_pointer {
public:
  typedef std::remove_reference_t<decltype(std::declval<T>().operator*())> type;
};

template <class T> using remove_pointer_t = typename remove_pointer<T>::type;
} // namespace type_traits

namespace iterator {

/*
 * Simple iterator wrapper to provide iteration over some range
 */
template <class I> class IteratorRange {
private:
  const I beginIt;
  const I endIt;

public:
  IteratorRange(const I &&, const I &&);

  const I begin() const;
  const I end() const;

  using iterator = I;
};

template <class I>
IteratorRange<I>::IteratorRange(const I &&begin, const I &&end)
    : beginIt(std::move(begin)), endIt(std::move(end)) {}

template <class I> const I IteratorRange<I>::begin() const {
  return this->beginIt;
}

template <class I> const I IteratorRange<I>::end() const { return this->endIt; }

template <class I> class DereferencingIterator {
  I iterator;

public:
  using value_type = type_traits::remove_pointer_t<typename I::value_type>;
  using difference_type = typename I::difference_type;
  using pointer = value_type *;
  using reference = value_type &;
  using iterator_category = std::input_iterator_tag;

  DereferencingIterator(I &&iterator);

  bool operator==(const DereferencingIterator &other) const;
  bool operator!=(const DereferencingIterator &other) const;

  DereferencingIterator &operator++();
  DereferencingIterator operator++(int);

  reference operator*();
  pointer operator->();
};

template <class I>
DereferencingIterator<I>::DereferencingIterator(I &&iterator)
    : iterator(std::move(iterator)) {}

template <class I>
bool DereferencingIterator<I>::operator==(
    const DereferencingIterator<I> &other) const {
  return this->iterator == other.iterator;
}

template <class I>
bool DereferencingIterator<I>::operator!=(
    const DereferencingIterator<I> &other) const {
  return this->iterator != other.iterator;
}

template <class I>
DereferencingIterator<I> &DereferencingIterator<I>::operator++() {
  ++this->iterator;
  return *this;
}

template <class I>
DereferencingIterator<I> DereferencingIterator<I>::operator++(int) {
  DereferencingIterator result = *this;
  ++this->iterator;
  return result;
}

template <class I>
typename DereferencingIterator<I>::reference
DereferencingIterator<I>::operator*() {
  return *(*this->iterator);
}

template <class I>
typename DereferencingIterator<I>::pointer
DereferencingIterator<I>::operator->() {
  return (this->iterator.operator->())->operator->();
}

} // namespace iterator

namespace graph {

/*
 * Interface of the graph framework
 */

using IdT = int;
const IdT InvalidId = -1;

class IEdge;

class IVertex {
public:
  virtual ~IVertex() = default;

  virtual operator IdT() const = 0;
};

class IEdge {
public:
  virtual ~IEdge() = default;

  virtual const IVertex &u() const = 0;
  virtual const IVertex &v() const = 0;

  virtual const IVertex &traverseFrom(const IVertex &) const = 0;
  virtual bool connects(const IVertex &, const IVertex &) const = 0;

  virtual operator IdT() const = 0;
};

class IGraph {
public:
  virtual ~IGraph() = default;

  virtual IVertex &vertex(const IdT &) const = 0;
  virtual IEdge &edge(const IdT &) const = 0;

  virtual IVertex &addVertex() = 0;
  virtual IEdge &addEdge(const IdT &, const IdT &) = 0;

  virtual std::size_t numVertices() const = 0;
  virtual std::size_t numEdges() const = 0;
};

/*
 * Implementation to be moved into different .h and .cpp files
 */

/*
 * Base vertex class which is linked to a graph with edge type E
 */
template <class G> class BaseVertex : public IVertex {
protected:
  const G *g;
  const IdT v;

public:
  // Constant representing invalid vertex of this type
  static BaseVertex<G> Invalid;

  // Construct invalid vertex which isn't linked to any graph
  BaseVertex();

  // Vertices can only be constructed by graph
  BaseVertex(const BaseVertex &other) = delete;

  // Constructor which is supposed to be used only by graph
  // Idk how to control access rights since you cannot friend all types of
  // graphs Since friendship isn't inherited
  BaseVertex(const G *, const IdT &);

  // Get outgoing _edges from this vertex in the graph this vertex is from
  const std::vector<std::reference_wrapper<typename G::Edge>> &
  outgoingEdges() const;

  operator IdT() const;
};

template <class G> BaseVertex<G> BaseVertex<G>::Invalid{};

template <class G> BaseVertex<G>::BaseVertex() : g(nullptr), v(InvalidId) {}

template <class G>
BaseVertex<G>::BaseVertex(const G *g, const IdT &v) : g(g), v(std::move(v)) {}

template <class G> BaseVertex<G>::operator IdT() const { return this->v; }

template <class G>
const std::vector<std::reference_wrapper<typename G::Edge>> &
BaseVertex<G>::outgoingEdges() const {
  return this->g->outgoingEdgesFrom(*this);
}

/*
 * Abstract base edge class which forms a graph with given vertex type
 */
template <class G> class BaseEdge : public IEdge {
protected:
  const IdT e;
  const typename G::Vertex &first;
  const typename G::Vertex &second;

public:
  // Constructs invalid edge
  BaseEdge();

  // Edges can only be constructed by graph
  BaseEdge(const BaseEdge &other) = delete;

  BaseEdge(const IdT &, const typename G::Vertex &, const typename G::Vertex &);

  const typename G::Vertex &u() const override;
  const typename G::Vertex &v() const override;

  operator IdT() const override;
};

template <class G>
BaseEdge<G>::BaseEdge()
    : e(InvalidId), first(G::Vertex::Invalid), second(G::Vertex::Invalid) {}

template <class G>
BaseEdge<G>::BaseEdge(const IdT &e, const typename G::Vertex &first,
                      const typename G::Vertex &second)
    : e(e), first(first), second(second) {}

template <class G> const typename G::Vertex &BaseEdge<G>::u() const {
  return this->first;
}

template <class G> const typename G::Vertex &BaseEdge<G>::v() const {
  return this->second;
}

template <class G> BaseEdge<G>::operator IdT() const { return this->e; }

/*
 * Edge class representing an undirected edge,
 * traversal of which can be done from both incident _vertices
 */
template <class G> class UndirectedEdge : public BaseEdge<G> {
public:
  static UndirectedEdge<G> Invalid;

  using BaseEdge<G>::BaseEdge;

  const typename G::Vertex &traverseFrom(const IVertex &) const override;

  bool connects(const IVertex &, const IVertex &) const override;
};

template <class G> UndirectedEdge<G> UndirectedEdge<G>::Invalid{};

template <class G>
const typename G::Vertex &
UndirectedEdge<G>::traverseFrom(const IVertex &v) const {
  if (v == this->first) {
    return this->second;
  } else if (v == this->second) {
    return this->first;
  }
  return G::Vertex::Invalid;
}

template <class G>
bool UndirectedEdge<G>::connects(const IVertex &u, const IVertex &v) const {
  return (this->first == u && this->second == v) ||
         (this->first == v && this->second == u);
}

template <template <class> class V, template <class> class E>
class AdjacencyListGraph : public IGraph {
public:
  using Vertex = V<AdjacencyListGraph>;
  using ConstVertexRef = std::reference_wrapper<const Vertex>;
  using VertexRef = std::reference_wrapper<Vertex>;
  using Edge = E<AdjacencyListGraph>;
  using ConstEdgeRef = std::reference_wrapper<const Edge>;
  using EdgeRef = std::reference_wrapper<Edge>;
  using VertexIterable =
      iterator::IteratorRange<iterator::DereferencingIterator<
          typename std::vector<std::unique_ptr<Vertex>>::iterator>>;
  using EdgeIterable = iterator::IteratorRange<iterator::DereferencingIterator<
      typename std::vector<std::unique_ptr<Edge>>::iterator>>;

protected:
  std::vector<std::vector<EdgeRef>> adjacent;
  std::vector<std::unique_ptr<Edge>> _edges;
  std::vector<std::unique_ptr<Vertex>> _vertices;

public:
  AdjacencyListGraph() = default;
  // Reserve enough places for all _vertices
  AdjacencyListGraph(const std::size_t);
  // Reserve enough places for all _vertices AND _edges
  // (not always known which is why this is a separate constructor)
  AdjacencyListGraph(const std::size_t, const std::size_t);

  Vertex &vertex(const IdT &) const override;
  Edge &edge(const IdT &) const override;

  Vertex &addVertex() override;
  Edge &addEdge(const IdT &, const IdT &) override;

  std::size_t numVertices() const override;
  std::size_t numEdges() const override;

  const std::vector<std::reference_wrapper<Edge>> &
  outgoingEdgesFrom(const IdT &) const;

  VertexIterable vertices();
  EdgeIterable edges();

private:
  template <class T> static bool validId(const T &, const IdT &);
};

template <template <class> class V, template <class> class E>
template <class T>
bool AdjacencyListGraph<V, E>::validId(const T &v, const IdT &id) {
  return id < IdT(v.size()) && id >= 0;
}

template <template <class> class V, template <class> class E>
AdjacencyListGraph<V, E>::AdjacencyListGraph(const std::size_t vertexNum) {
  adjacent.resize(vertexNum);
  _vertices.reserve(vertexNum);
  for (std::size_t i = 0; i < vertexNum; i++) {
    this->addVertex();
  }
}

template <template <class> class V, template <class> class E>
AdjacencyListGraph<V, E>::AdjacencyListGraph(const std::size_t vertexNum,
                                             const std::size_t edgeNum)
    : AdjacencyListGraph(vertexNum) {
  _edges.reserve(edgeNum);
}

template <template <class> class V, template <class> class E>
typename AdjacencyListGraph<V, E>::Vertex &
AdjacencyListGraph<V, E>::vertex(const IdT &v) const {
  return *this->_vertices.at(v);
}

template <template <class> class V, template <class> class E>
typename AdjacencyListGraph<V, E>::Edge &
AdjacencyListGraph<V, E>::edge(const IdT &e) const {
  return *this->_edges.at(e);
}

template <template <class> class V, template <class> class E>
typename AdjacencyListGraph<V, E>::Vertex &
AdjacencyListGraph<V, E>::addVertex() {
  this->_vertices.push_back(
      std::make_unique<Vertex>(this, std::move(this->_vertices.size())));
  return *this->_vertices.back();
}

template <template <class> class V, template <class> class E>
typename AdjacencyListGraph<V, E>::Edge &
AdjacencyListGraph<V, E>::addEdge(const IdT &u, const IdT &v) {
  if (this->template validId(_vertices, u) &&
      this->template validId(_vertices, v)) {
    auto &vertexU{this->vertex(u)};
    auto &vertexV{this->vertex(v)};
    this->_edges.push_back(
        std::make_unique<Edge>(this->_edges.size(), vertexU, vertexV));
    auto &newEdge = *this->_edges.back();
    if (newEdge.connects(vertexU, vertexV)) {
      this->adjacent[u].push_back(newEdge);
    }
    if (newEdge.connects(vertexV, vertexU)) {
      this->adjacent[v].push_back(newEdge);
    }
    return *this->_edges.back();
  }
  return Edge::Invalid;
}

template <template <class> class V, template <class> class E>
std::size_t AdjacencyListGraph<V, E>::numVertices() const {
  return this->_vertices.size();
}

template <template <class> class V, template <class> class E>
std::size_t AdjacencyListGraph<V, E>::numEdges() const {
  return this->_edges.size();
}

template <template <class> class V, template <class> class E>
const std::vector<
    std::reference_wrapper<typename AdjacencyListGraph<V, E>::Edge>> &
AdjacencyListGraph<V, E>::outgoingEdgesFrom(const IdT &u) const {
  return this->adjacent.at(u);
}

template <template <class> class V, template <class> class E>
typename AdjacencyListGraph<V, E>::VertexIterable
AdjacencyListGraph<V, E>::vertices() {
  return iterator::IteratorRange(
      iterator::DereferencingIterator(_vertices.begin()),
      iterator::DereferencingIterator(_vertices.end()));
}

template <template <class> class V, template <class> class E>
typename AdjacencyListGraph<V, E>::EdgeIterable
AdjacencyListGraph<V, E>::edges() {
  return iterator::IteratorRange(
      iterator::DereferencingIterator(_edges.begin()),
      iterator::DereferencingIterator(_edges.end()));
}

} // namespace graph

} // namespace cppren

using namespace cppren::graph;
using Graph = AdjacencyListGraph<BaseVertex, UndirectedEdge>;

using intVec = std::vector<int>;

std::vector<Graph::ConstEdgeRef> findBridges(Graph &g) {
  intVec tin(g.numVertices(), -1);
  intVec low(g.numVertices(), -1);
  std::vector<Graph::ConstEdgeRef> bridges;
  int time = 0;

  std::function<void(const Graph::Vertex &v, const Graph::Edge &e)> internal;
  internal = [&](const Graph::Vertex &v, const Graph::Edge &edge) {
    tin[v] = low[v] = time++;
    for (const Graph::Edge &e : v.outgoingEdges()) {
      if (e == edge) {
        continue;
      }
      auto &to = e.traverseFrom(v);
      if (tin[to] == -1) {
        internal(to, e);
        low[v] = std::min(low[v], low[to]);
        if (low[to] > tin[v]) {
          bridges.push_back(std::ref(e));
        }
      } else {
        low[v] = std::min(low[v], tin[to]);
      }
    }
  };

  for (Graph::Vertex &v : g.vertices()) {
    if (tin[v] == -1) {
      internal(v, Graph::Edge::Invalid);
    }
  }

  return bridges;
}

std::pair<std::vector<int>, int> markEdgeBiconnected(Graph &g) {
  auto bridges = findBridges(g);
  std::unordered_set<int> bridge_set(bridges.size());
  for (auto &e : bridges) {
    bridge_set.emplace(int(e.get()));
  }
  std::vector<int> marked(g.numVertices(), -1);
  int component = 0;

  std::function<void(const Graph::Vertex &v, const int current_component)>
      internal;
  internal = [&](const Graph::Vertex &v, const int currentComponent) {
    marked[v] = currentComponent;
    for (const Graph::Edge &e : v.outgoingEdges()) {
      const auto &to = e.traverseFrom(v);
      if (marked[to] == -1) {
        if (bridge_set.contains(e)) {
          component++;
          internal(to, component);
        } else {
          internal(to, currentComponent);
        }
      }
    }
  };

  for (Graph::Vertex &v : g.vertices()) {
    if (marked[v] == -1) {
      component++;
      internal(v, component);
    }
  }

  return {marked, component};
}

int main() {
  int n, m;
  std::cin >> n >> m;
  Graph g(n, m);
  for (int i = 0; i < m; i++) {
    int u, v;
    std::cin >> u >> v;
    g.addEdge(u - 1, v - 1);
  }
  const auto [marked, component] = markEdgeBiconnected(g);
  std::cout << component << std::endl;
  for (int i : marked) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  return 0;
}