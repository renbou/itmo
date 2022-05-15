#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

using int64 = int64_t;
using graph = vector<vector<int64>>;

graph g;
graph gt;

void dfsVisit(graph &g, vector<bool> &visited, int v, int64 weight) {
  visited[v] = true;
  for (int to = 0; to < int(g.size()); to++) {
    if (!visited[to] && g[v][to] <= weight) {
      dfsVisit(g, visited, to, weight);
    }
  }
}

bool allVisited(graph &g, int64 weight) {
  vector<bool> visited(g.size(), false);
  dfsVisit(g, visited, 0, weight);
  return all_of(visited.begin(), visited.end(),
                [](bool value) { return value; });
}

bool isStronglyConnected(int64 weight) {
  return allVisited(g, weight) && allVisited(gt, weight);
}

int64 binarySearch() {
  int64 l = -1, r = 1'000'000'000;
  while (r - l > 1) {
    int64 mid = l + (r - l) / 2;
    if (isStronglyConnected(mid)) {
      r = mid;
    } else {
      l = mid;
    }
  }
  return r;
}

int main() {
  int n;
  cin >> n;
  g.assign(n, vector<int64>(n, -1));
  gt.assign(n, vector<int64>(n, -1));

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      cin >> g[i][j];
      gt[j][i] = g[i][j];
    }
  }
  cout << binarySearch() << endl;
}