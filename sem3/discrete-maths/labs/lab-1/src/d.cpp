#include <algorithm>
#include <array>
#include <ios>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <random>
#include <set>
#include <stdexcept>
#include <unordered_set>
#include <vector>
 
using uint64 = uint64_t;
using uint32 = uint32_t;
using int64 = int64_t;
using int32 = int32_t;
 
using std::array;
using std::endl, std::cin, std::cout;
using std::list;
using std::unordered_set;
using std::vector;
 
using Adjacent = vector<vector<bool>>;
using Graph = vector<vector<int>>;
 
vector<int> buildHamiltonianPath(const Adjacent &adjacent) {
  const int n = adjacent.size();
  list<int> path;
  path.push_back(0);

  for (int v = 1; v < n; v++) {
    auto it = path.begin();
    while (it != path.end() && adjacent[*it][v]) {
      ++it;
    }
    path.insert(it, v);
  }
 
  return vector<int>(path.begin(), path.end());
}
 
vector<int> pathToCycle(const Adjacent &adjacent, const vector<int> &path) {
  if (adjacent[path.back()][path.front()]) {
    // if already a cycle
    return path;
  }
 
  const int n = adjacent.size();
  list<int> cycle;

  // find last valid end of cycle (which connects to beginning)
  int cycleEnd = n - 1;
  while (cycleEnd > 1 && !adjacent[path[cycleEnd]][path.front()]) {
    --cycleEnd;
  }
  cycle.insert(cycle.end(), path.begin(), path.begin() + cycleEnd + 1);
 
  auto findAdjacent = [&](const int v) {
    auto answer = cycle.begin();
    while (!adjacent[v][*answer] && answer != cycle.end()) {
      ++answer;
    }
    return answer;
  };
 
  vector<int> buffer;
  buffer.reserve(n - cycle.size());
  for (int k = cycle.size(); k < n; k++) {
    auto adjacentV = findAdjacent(path[k]);
    if (adjacentV == cycle.end()) {
      // temporarily buffer this vertice
      buffer.push_back(path[k]);
    } else {
      // when we find a valid vertex we can add all buffered ones
      // since for all buffered vertices we have that all vertices in the
      // cycle point into them, thus adjacent-1 points to the beginning
      // then we have the buffer and then k -> adjacent
      cycle.insert(adjacentV, buffer.begin(), buffer.end());
      buffer.clear();
      cycle.insert(adjacentV, path[k]);
    }
  }
 
  return vector<int>(cycle.begin(), cycle.end());
}
 
int main() {
  cin.tie(nullptr);
  cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
 
  int n;
  cin >> n;
 
  Adjacent adjacent(n, vector<bool>(n, false));
 
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      char c;
      cin >> c;
      if (c == '1') {
        adjacent[i][j] = true;
      } else {
        adjacent[j][i] = true;
      }
    }
  }
 
  const auto hamiltonianPath = buildHamiltonianPath(adjacent);
  const auto hamiltonianCycle = pathToCycle(adjacent, hamiltonianPath);
  for (int v : hamiltonianCycle) {
    std::cout << v + 1 << ' ';
  }
  std::cout << std::endl;
}