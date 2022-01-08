#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
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

int main() {
  freopen("schedule.in", "r", stdin);
  freopen("schedule.out", "w", stdout);

  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);
  const auto io = IO(std::cin, std::cout);

  const int n = io.read<int>();

  std::vector<int> deadlines(n, -1);
  std::unordered_map<int, std::vector<int64>> tasksByDeadline;

  for (const int i : Range(n)) {
    const int deadline = io.read<int>();
    deadlines[i] = deadline;
    if (!tasksByDeadline.count(deadline)) {
      tasksByDeadline.emplace(deadline, std::vector<int64>{});
    }
    tasksByDeadline[deadline].emplace_back(io.read<int>());
  }

  // uniqueify deadlines
  std::sort(deadlines.begin(), deadlines.end());
  deadlines.resize(std::distance(
      deadlines.begin(), std::unique(deadlines.begin(), deadlines.end())));

  std::priority_queue<int64> tasks;
  int currentTime = deadlines.back();
  int nextDeadlineIndex = deadlines.size() - 1;
  while (currentTime > 0) {
    // if we have reached a new deadline, add all tasks for this deadline
    // since there might be a task we want to complete right now
    if (nextDeadlineIndex >= 0 && currentTime == deadlines[nextDeadlineIndex]) {
      for (const auto task : tasksByDeadline[currentTime]) {
        tasks.emplace(task);
      }
      nextDeadlineIndex--;
    }

    // if we have completed all tasks there is no point in decrementing
    // currentTime by 1, skip over to the next deadline
    if (tasks.empty()) {
      if (nextDeadlineIndex >= 0) {
        currentTime = deadlines[nextDeadlineIndex];
      } else {
        currentTime = 0;
      }
      continue;
    }

    // complete heaviest task and go to next timestamp
    tasks.pop();
    currentTime--;
  }

  // all other tasks haven't been done
  int64 penalty = 0;
  while (!tasks.empty()) {
    auto task = tasks.top();
    tasks.pop();
    penalty += task;
  }
  while (nextDeadlineIndex >= 0) {
    for (const auto task : tasksByDeadline[deadlines[nextDeadlineIndex]]) {
      penalty += task;
    }
    nextDeadlineIndex--;
  }

  io.write(penalty);
  io.nl();
}