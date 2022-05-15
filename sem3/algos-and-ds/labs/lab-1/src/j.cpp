#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>
#include <vector>

using int64 = std::int64_t;
using std::cin, std::cout, std::endl;
using std::set;
using std::vector;

struct Edge {
  int64 weight;
  int v;

  bool operator<(const Edge &other) const {
    if (other.weight != weight) {
      return weight > other.weight;
    }
    return v > other.v;
  }
};

int64 sumMST(vector<vector<Edge>> &g) {
  const int n = g.size();
  const int64 oo = 1'000'000'000;

  std::priority_queue<Edge> queue;
  vector<int64> minEdge(n, oo);
  vector<bool> selected(n, false);

  minEdge[0] = 0;
  queue.push({0, 0});

  int64 sum = 0;
  while (!queue.empty()) {
    auto [weight, u] = queue.top();
    queue.pop();
    if (selected[u]) {
      continue;
    }

    selected[u] = true;
    sum += weight;

    for (const Edge &e : g.at(u)) {
      if (!selected[e.v] and e.weight < minEdge[e.v]) {
        minEdge[e.v] = e.weight;
        queue.push({e.weight, e.v});
      }
    }
  }

  return sum;
}

int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<Edge>> g(n);
  for (int i = 0; i < m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    g.at(u - 1).push_back({w, v - 1});
    g.at(v - 1).push_back({w, u - 1});
  }

  if (n == 0) {
    cout << 0 << endl;
    return 0;
  }

  cout << sumMST(g) << endl;
  return 0;
}