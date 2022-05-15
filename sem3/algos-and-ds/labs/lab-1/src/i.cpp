#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <ios>
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

long double dist(std::pair<int, int> a, std::pair<int, int> b) {
  long double xd = a.first - b.first;
  long double yd = a.second - b.second;
  return sqrtl(xd * xd + yd * yd);
}

long double sumMST(vector<std::pair<int, int>> &g) {
  const int n = g.size();
  const int64 oo = 1'000'000'000;

  vector<long double> minEdge(n, oo);
  vector<bool> selected(n, false);

  minEdge[0] = 0;

  long double sum = 0;
  for (int i = 0; i < n; i++) {
    int u = -1;
    for (int v = 0; v < n; v++) {
      if (not selected[v] and (u == -1 or minEdge[v] < minEdge[u])) {
        u = v;
      }
    }

    selected[u] = true;
    sum += minEdge[u];

    for (int v = 0; v < n; v++) {
      if (const long double d = dist(g[u], g[v]); d < minEdge[v]) {
        minEdge[v] = d;
      }
    }
  }

  return sum;
}

int main() {
  int n;
  cin >> n;
  vector<std::pair<int, int>> g(n);
  for (int i = 0; i < n; i++) {
    int u, v;
    cin >> u >> v;
    g[i] = {u, v};
  }

  if (n == 0) {
    cout << 0 << endl;
    return 0;
  }

  cout << std::setprecision(15) << std::fixed << sumMST(g) << endl;
  return 0;
}