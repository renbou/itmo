#include <iostream>
#include <vector>

bool existsPath(int u, int v) {
  std::cout << 1 << ' ' << u << ' ' << v << std::endl;
  std::string answer;
  std::cin >> answer;
  return answer[0] == 'Y';
}

int main() {
  int n;
  std::cin >> n;
  std::vector<int> result = { 1 };
  result.reserve(n);
  for (int v = 2; v <= n; v++) {
    int l = -1;
    int r = v - 1;
    // Find valid l, r such that the invariant
    // L -> V -> R holds
    while (r - l > 1) {
      int m = l + (r - l) / 2;
      if (existsPath(result[m], v)) {
        // L -> V is now always valid
        l = m;
      } else {
        // V -> R is now always valid
        r = m;
      }
    }
    result.insert(result.begin() + r, v);
  }
  std::cout << 0 << ' ';
  for (int v: result) {
    std::cout << v << ' ';
  }
  std::cout << std::endl;
}