#pragma GCC optimize("O1")

#include <algorithm>
#include <array>
#include <ios>
#include <iostream>
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
using std::unordered_set;
using std::vector;

using Adjacent = vector<vector<bool>>;
using Graph = vector<vector<int>>;

void *mem;

void throwOOM() {
  vector<char *> mem(1000);
  for (int i = 0; i < 1000; i++) {
    mem[i] = (char *)calloc(1 * 1024 * 1024, 1);
  }
  for (char *p : mem) {
    for (int i = 0; i < 1 * 1024 * 1024; i++) {
      if (p[i] != 0) {
        throw std::runtime_error("invalid memory");
      }
    }
  }
}

struct DynAdjacent {
  Adjacent adjacent;
  vector<int> nextAvailable;
};

// find vertex not yet in hamiltonian path that is a neighbour of v
int findAvailableNeighbour(DynAdjacent &dAdjacent, int v) {
  return dAdjacent.nextAvailable[v];
}

// disconnects v from every neighbour
void disconnectFromNeighbours(Graph &g, DynAdjacent &dAdjacent, int v) {
  const int n = g.size();

  for (int u : g[v]) {
    dAdjacent.adjacent[u][v] = false;
    if (dAdjacent.nextAvailable[u] == v) {
      // need to update next available
      dAdjacent.nextAvailable[u] = -1;
      // this executes maximum n times during the whole program, since v always
      // increases
      for (int av = v + 1; av < n; av++) {
        if (dAdjacent.adjacent[u][av]) {
          dAdjacent.nextAvailable[u] = av;
          break;
        }
      }
    }
  }
}

std::pair<vector<int>, bool> findHamiltonianCycle(Adjacent &adjacent, Graph &g,
                                                  DynAdjacent &dAdjacent) {
  const int n = g.size();

  vector<int> cycle;
  cycle.reserve(n);
  // location of v in cycle
  vector<int> location(n, -1);

  auto addToCycle = [&](int v) {
    disconnectFromNeighbours(g, dAdjacent, v);
    location[v] = cycle.size();
    cycle.push_back(v);
  };

  // lets start from the first vertex
  addToCycle(0);

  for (int i = 1; i < n; ++i) {
    const int v = cycle.back();
    const int neighbour = findAvailableNeighbour(dAdjacent, v);
    if (neighbour != -1) {
      // we found a valid neighbour, lets remove it from other vertices
      addToCycle(neighbour);
      continue;
    }

    int next = -1;
    int nextNeighbour = -1;
    // Otherwise lets find a valid neighbour seen before
    for (int neighbour : g[v]) {
      next = cycle[location[neighbour] + 1];
      nextNeighbour = findAvailableNeighbour(dAdjacent, next);
      if (nextNeighbour != -1) {
        break;
      }
    }

    if (nextNeighbour == -1) {
      // throw std::runtime_error("cannot find valid neighbour");
      //  throwOOM();
      return {{}, false};
    }

    // Connect neighbour to v, disconnect next from neighbour and now there is a
    // new path ending in next which we know we can extend with nextNeighbour
    for (int f = location[next], e = i - 1; f < e; ++f, --e) {
      std::swap(location[cycle[f]], location[cycle[e]]);
      std::swap(cycle[f], cycle[e]);
    }

    // Now connect nextneighbour
    addToCycle(nextNeighbour);
  }

  if (!adjacent[cycle.back()][cycle.front()]) {
    // if last and first aren't adjacent then we need to reorder the beginning
    int first = cycle.front(), last = cycle.back();
    int neighbour = -1;
    for (int neighbourTry : g[last]) {
      const int neighbourNext = cycle[location[neighbourTry] + 1];
      if (adjacent[neighbourNext][first]) {
        // Found one
        neighbour = neighbourTry;
        break;
      }
    }

    if (neighbour == -1) {
      // wut?
      // throw std::runtime_error("cannot find valid neighbour");
      return {{}, false};
    }

    // connect neighbour to last, reverse path from neighbour to first,
    // and then connect first to neighbourNext
    std::reverse(cycle.begin(), cycle.begin() + location[neighbour] + 1);
  }

  return {cycle, true};
}

Adjacent buildAdjacent(Graph &g) {
  const int n = g.size();
  Adjacent adjacent(n, vector<bool>(n, false));
  for (int v = 0; v < n; v++) {
    for (int u : g[v]) {
      adjacent[v][u] = adjacent[u][v] = true;
    }
  }
  return adjacent;
}

void updateNextAvailable(DynAdjacent &dAdjacent, int u, int v) {
  if (dAdjacent.nextAvailable[u] > v) {
    dAdjacent.nextAvailable[u] = v;
  }
  if (dAdjacent.nextAvailable[v] > u) {
    dAdjacent.nextAvailable[v] = u;
  }
}

void buildDynAdjacent(Graph &g, DynAdjacent &dAdjacent) {
  const int n = g.size();
  dAdjacent.nextAvailable.assign(n, 1'000'000);
  for (int v = 0; v < n; v++) {
    for (int u : g[v]) {
      updateNextAvailable(dAdjacent, u, v);
    }
  }
}

auto rnd = std::mt19937{std::random_device{}()};

vector<int> randomPermutation(const int n) {
  vector<int> perm(n);
  std::iota(perm.begin(), perm.end(), 0);
  std::shuffle(perm.begin(), perm.end(), rnd);
  return perm;
}

Graph randomGraphPermutation(Graph &g, vector<int> perm) {
  const int n = g.size();
  Graph ng(n);

  for (int v = 0; v < n; v++) {
    for (int u : g[v]) {
      ng[perm[v]].push_back(perm[u]);
    }
  }
  return ng;
}

int main() {
  cin.tie(nullptr);
  cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n;
  cin >> n;

  Graph g(n);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      char c;
      cin >> c;
      if (c == '1') {
        g[i].push_back(j);
        g[j].push_back(i);
      }
    }
  }

  while (true) {
    vector<int> perm = randomPermutation(n);
    Graph ng = randomGraphPermutation(g, perm);
    DynAdjacent dAdjacent;
    Adjacent adjacent = buildAdjacent(ng);
    dAdjacent.adjacent = adjacent;
    buildDynAdjacent(ng, dAdjacent);

    vector<int> answer;
    bool valid;
    std::tie(answer, valid) = findHamiltonianCycle(adjacent, ng, dAdjacent);
    if (valid) {
      vector<int> reverse(n);
      for (int i = 0; i < n; i++) {
        reverse[perm[i]] = i;
      }
      for (int i = 0; i < n; i++) {
        answer[i] = reverse[answer[i]];
      }

      for (int v : answer) {
        std::cout << v + 1 << ' ';
      }
      return 0;
    }
  }
  return 0;
}