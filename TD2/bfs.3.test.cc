#include "bfs.3.h"
#include "main_utils.h"
#include <algorithm>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main() {
  cout << "Test: simple graph with one edge" << endl;
  CHECK_EQ(StrJoin(GetShortestPathFromRootedTree({0, 0}, 1), ", "), "0, 1");
  cout << "PASSED\n";

  cout << "Test: simple chain" << endl;
  CHECK_EQ(StrJoin(GetShortestPathFromRootedTree({1, 2, 3, 3}, 0), ", "), "3, 2, 1, 0");
  cout << "PASSED\n";

  cout << "Test: small graph, various paths" << endl;
  CHECK_EQ(StrJoin(GetShortestPathFromRootedTree({-1, 3, 2, 4, 5, 2, 3, 4, 6}, 8), ", "),
           "2, 5, 4, 3, 6, 8");
  CHECK_EQ(StrJoin(GetShortestPathFromRootedTree({-1, 3, 2, 4, 5, 2, 3, 4, 6}, 2), ", "),
           "2");
  CHECK_EQ(StrJoin(GetShortestPathFromRootedTree({-1, 3, 2, 4, 5, 2, 3, 4, 6}, 0), ", "),
           "");
  CHECK_EQ(StrJoin(GetShortestPathFromRootedTree({-1, 3, 2, 4, 5, 2, 3, 4, 6}, 1), ", "),
           "2, 5, 4, 3, 1");
  cout << "PASSED\n";

  cout << "Test: graph with only one node" << endl;
  CHECK_EQ(StrJoin(GetShortestPathFromRootedTree({0}, 0), ", "), "0");
  cout << "PASSED\n";

  cout << "Test: disconnected" << endl;
  CHECK_EQ(StrJoin(GetShortestPathFromRootedTree({0, -1}, 1), ", "), "");
  cout << "PASSED\n";

  {
    cout << "Performance + stress test (should take ~0.02s)" << endl;
    Random random(123);
    const int Ntot = 11000;
    const int N = 10000;
    const int maxdist = 300;
    vector<int> nodes(Ntot, 0);
    std::iota(nodes.begin(), nodes.end(), 0);
    shuffle(nodes.begin(), nodes.end(), random);
    const int src = nodes[0];
    vector<int> distance(Ntot, -1);
    vector<vector<int>> nodes_at_distance(maxdist + 1);
    vector<int> parent(Ntot, -1);
    int last = 0;
    int cur = 1;
    nodes_at_distance[0].push_back(src);
    distance[src] = 0;
    parent[src] = src;
    for (int d = 1; d <= maxdist; ++d) {
      const int mini = d == maxdist ? N - cur : 1;
      const int maxi = N - (maxdist - d) - cur;
      CHECK_GE(maxi, mini);
      const int num = random.Uniform(maxi - mini + 1) + mini;
      for (int i = cur; i < cur + num; ++i) {
        distance[nodes[i]] = d;
        nodes_at_distance[d].push_back(nodes[i]);
        parent[nodes[i]] = nodes[random.Uniform(cur - last) + last];
      }
      last = cur;
      cur += num;
    }
    clock_t c0 = clock();
    for (int i = 0; i < Ntot; ++i) {
      const vector<int> path = GetShortestPathFromRootedTree(parent, i);
      for (int t = 0; t < 10; ++t) {
        CHECK_EQ(GetShortestPathFromRootedTree(parent, i),
                 path);
      }
      if (parent[i] == -1) {
        CHECK_EQ(path.size(), 0);
        continue;
      }
      CHECK_GT(path.size(), 0);
      CHECK_EQ(path[0], src);
      CHECK_EQ(path.back(), i);
      for (int x : path) {
        CHECK_GE(x, 0);
        CHECK_LT(x, Ntot);
      }
      for (int i = 1; i < path.size(); ++i) {
        CHECK_EQ(distance[path[i - 1]] + 1,
                 distance[path[i]]);
      }
    }
    double t = double(clock() - c0) / CLOCKS_PER_SEC;
    cout << "Time: " << t << "s\n";
    cout << "PASSED\n";
  }
}
