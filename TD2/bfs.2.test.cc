#include "bfs.2.h"
#include "main_utils.h"
#include <algorithm>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main() {
  {
    cout << "Test: simple graph with one edge" << endl;
    CHECK_EQ(StrJoin(GetBfsDistances({0, 0}), ", "), "0, 1");
    cout << "PASSED\n";

    cout << "Test: simple 'star' graph" << endl;
    CHECK_EQ(StrJoin(GetBfsDistances({1, 1, 1, 1}), ", "), "1, 0, 1, 1");
    cout << "PASSED\n";

    cout << "Test: small chain + disconnected node" << endl;
    CHECK_EQ(StrJoin(GetBfsDistances({-1, 3, 2, 2}), ", "), "-1, 2, 0, 1");
    cout << "PASSED\n";

    cout << "Test: small example" << endl;
    CHECK_EQ(StrJoin(GetBfsDistances({-1, 3, 2, 4, 5, 2, 3, 4, 6}), ", "),
           "-1, 4, 0, 3, 2, 1, 4, 3, 5");
    cout << "PASSED\n";

    cout << "Test: graph with only one node" << endl;
    CHECK_EQ(StrJoin(GetBfsDistances({0}), ", "), "0");
    cout << "PASSED\n";
  
    cout << "Test: empty graph (zero node)" << endl;
    CHECK_EQ(StrJoin(GetBfsDistances({}), ", "), "");
    cout << "PASSED\n";
  
    cout << "Test: all disconnected but the source" << endl;
    CHECK_EQ(StrJoin(GetBfsDistances({-1, -1, -1, 3, -1, -1}), ", "),
             "-1, -1, -1, 0, -1, -1");
    cout << "PASSED\n";
  }
  {
    cout << "Performance + stress test" << endl;
    Random random(123);
    const int N = 30000;
    const int maxdist = 3000;
    vector<int> nodes(N, 0);
    std::iota(nodes.begin(), nodes.end(), 0);
    shuffle(nodes.begin(), nodes.end(), random);
    const int src = nodes[0];
    vector<int> distance(N, 0);
    vector<vector<int>> nodes_at_distance(maxdist + 1);
    vector<int> parent(N, 0);
    int last = 0;
    int cur = 1;
    nodes_at_distance[0].push_back(src);
    parent[src] = src;
    for (int d = 1; d <= maxdist; ++d) {
      const int mini = d == maxdist ? nodes.size() - cur : 1;
      const int maxi = nodes.size() - (maxdist - d) - cur;
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
    const vector<int> d = GetBfsDistances(parent);
    double t = double(clock() - c0) / CLOCKS_PER_SEC;
    cout << "Time: " << t << "s\n";
    CHECK_EQ(d, distance);
    cout << "PASSED\n";
  }
}
