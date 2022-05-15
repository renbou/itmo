#include <iostream>
#include <queue>
#include <vector>

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<int> inDegree(n, 0);
  std::vector<std::vector<int>> g(n);
  for (int i = 0; i < n; i++) {
    int v, u;
    std::cin >> v >> u;
    g[v - 1].push_back(u - 1);
    inDegree[u - 1]++;
  }

  std::queue<int> q;
  for (int i = 0; i < n; i++) {
    if (inDegree[i] == 0) {
      q.push(i);
    }
  }
  int numProcessed = 0;
  std::vector<int> topsort;
  topsort.reserve(n);
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    topsort.push_back(v);

    for (int to : g[v]) {
      if (--inDegree[to] == 0) {
        q.push(to);
      }
    }

    numProcessed++;
  }

  if (numProcessed != n) {
    std::cout << -1;
  } else {
    for (int v : topsort) {
      std::cout << v + 1 << " ";
    }
  }
  std::cout << std::endl;
}