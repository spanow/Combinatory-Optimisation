#include "ugraph.h"
#include "bfs.1.h"
#include "main_utils.h"
#include <algorithm>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main() {
  {
    cout << "Simple examples." << endl;

    UndirectedGraph g(5);
    CHECK_EQ("[0 -1 -1 -1 -1]", ToStr(BFS(g, 0)));
    cout << "PASSED at line " << __LINE__ << endl;
    CHECK_EQ("[-1 -1 2 -1 -1]", ToStr(BFS(g, 2)));
    cout << "PASSED at line " << __LINE__ << endl;

    g.AddEdge(1, 0);
    CHECK_EQ("[0 0 -1 -1 -1]", ToStr(BFS(g, 0)));
    cout << "PASSED at line " << __LINE__ << endl;
    CHECK_EQ("[1 1 -1 -1 -1]", ToStr(BFS(g, 1)));
    cout << "PASSED at line " << __LINE__ << endl;
    
    g.AddEdge(1, 2);
    g.AddEdge(3, 2);
    g.AddEdge(3, 4);
    CHECK_EQ("[0 0 1 2 3]", ToStr(BFS(g, 0)));
    cout << "PASSED at line " << __LINE__ << endl;
    CHECK_EQ("[1 2 3 4 4]", ToStr(BFS(g, 4)));
    cout << "PASSED at line " << __LINE__ << endl;
    CHECK_EQ("[1 2 2 2 3]", ToStr(BFS(g, 2)));
    cout << "PASSED at line " << __LINE__ << endl;
   
    // Add a cycle. Beware, infinite loops!
    g.AddEdge(4, 0);
    CHECK_EQ("[0 0 1 4 0]", ToStr(BFS(g, 0)));
    cout << "PASSED at line " << __LINE__ << endl;
    CHECK_EQ("[4 2 3 3 3]", ToStr(BFS(g, 3)));
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "More complex example." << endl;
    UndirectedGraph g(9);
    g.AddEdge(1, 0);
    g.AddEdge(1, 3);
    g.AddEdge(3, 1);
    g.AddEdge(2, 0);
    g.AddEdge(1, 2);
    g.AddEdge(5, 3);
    g.AddEdge(7, 4);
    g.AddEdge(7, 4);
    g.AddEdge(7, 3);
    g.AddEdge(8, 2);

    CHECK_EQ("[0 0 0 1 7 3 -1 3 2]", ToStr(BFS(g, 0)));
    CHECK_EQ("[1 1 1 1 7 3 -1 3 2]", ToStr(BFS(g, 1)));
    CHECK_EQ("[2 2 2 1 7 3 -1 3 2]", ToStr(BFS(g, 2)));
    CHECK_EQ("[1 3 1 3 7 3 -1 3 2]", ToStr(BFS(g, 3)));
    CHECK_EQ("[1 3 1 7 4 3 -1 4 2]", ToStr(BFS(g, 4)));
    CHECK_EQ("[1 3 1 5 7 5 -1 3 2]", ToStr(BFS(g, 5)));
    CHECK_EQ("[-1 -1 -1 -1 -1 -1 6 -1 -1]", ToStr(BFS(g, 6)));
    CHECK_EQ("[1 3 1 7 7 3 -1 7 2]", ToStr(BFS(g, 7)));

    cout << "PASSED!\n";
  }

  // Performance test.
  {
    cout << "Performance test" << endl;
    const int N = 10000;
    const int M = N * 2;
    UndirectedGraph g(N);
    Random random(123);
    for (int i = 0; i < M; ++i) {
      g.AddEdge(random.Uniform(N), random.Uniform(N));
    }
    const int times = 10;
    const clock_t c0 = clock();
    const int src = N / 2 + 42;
    const vector<int> bfs0 = BFS(g, src);
    int size = 0;
    for (int x : bfs0) if (x >= 0) ++size;
    CHECK_GE(size, N / 2);
    CHECK_LT(size, N);
    for (int t = 1; t < times; ++t) {
      CHECK_EQ(BFS(g, src), bfs0);
    }
    double t = double(clock() - c0) / CLOCKS_PER_SEC;
    cout << "Time: " << t << "s\n";
    if (t > 5) {
      cout << "Way too slow!" << endl;
      return 1;
    } else if (t > 0.1) {
      cout << "Slow! Your complexity is probably quadratic?" << endl;
      return 1;
    } else if (t > 0.02) {
      cout << "Slow! Is your complexity really linear?" << endl;
      return 1;
    } else {
      cout << "PASSED! Your complexity is likely linear.\n";
    }
  }

  // Correctness test, on a "random" graph constructed to obey a
  // predefined (randomized) set of distances from the source.
  {
    cout << "Randomized Stress test" << endl;
    const clock_t c0 = clock();
    const int trials = 1000;
    Random random(123);
    for (int i = 0; i < trials; ++i) {
      const int N = random.Uniform(20) + 1;
      UndirectedGraph g(N);
      int M = N + random.Uniform(N * 2);
      if (random.RandInt() & 1) M = max(N, N*N - M);
      const int maxdist = N <= 1 ? 0 : 1 + random.Uniform(N - 1);
      //cout << "Trial #" << i << ": N=" << N << ", maxdist=" << maxdist << ", M=" << M << endl;
      vector<int> nodes(N, 0);
      vector<int> distance(N, 0);
      vector<vector<int>> nodes_at_distance(maxdist + 1);
      vector<pair<int, int>> edges;
      std::iota(nodes.begin(), nodes.end(), 0);
      shuffle(nodes.begin(), nodes.end(), random);
      const int src = nodes[0];
      int last = 0;
      int cur = 1;
      nodes_at_distance[0].push_back(src);
      for (int d = 1; d <= maxdist; ++d) {
        //cout << d << endl;
        const int mini = d == maxdist ? nodes.size() - cur : 1;
        const int maxi = nodes.size() - (maxdist - d) - cur;
        CHECK_GE(maxi, mini);
        const int num = random.Uniform(maxi - mini + 1) + mini;
        for (int i = cur; i < cur + num; ++i) {
          distance[nodes[i]] = d;
          nodes_at_distance[d].push_back(nodes[i]);
          edges.push_back({nodes[i], nodes[random.Uniform(cur - last) + last]});
          //cout << "  " << edges.back().first << " - " << edges.back().second << endl;
        }
        last = cur;
        cur += num;
      }
      while (edges.size() < M) {
        int a = random.Uniform(N);
        int d = random.Uniform(2) + distance[a];
        if (d > maxdist) d = max(0, maxdist - 1);
        int b = nodes_at_distance[d][random.Uniform(nodes_at_distance[d].size())];
        edges.push_back({a, b});
      }
      shuffle(edges.begin(), edges.end(), random);
      for (auto& e : edges) {
        if (random.RandInt() & 1) std::swap(e.first, e.second);
        g.AddEdge(e.first, e.second);
        CHECK_LE(std::abs(distance[e.first] - distance[e.second]), 1);
      }

      const vector<int> parent = BFS(g, src);
      CHECK_EQ(parent.size(), N);
      for (int i = 0; i < N; ++i) {
        const int p = parent[i];
        CHECK_GE(p, 0);
        CHECK_LT(p, N);
        if (distance[i] == 0) {
          CHECK_EQ(i, src);
          CHECK_EQ(p, i);
          continue;
        } else {
          CHECK_EQ(distance[p], distance[i] - 1);
        }
      }
    }
    cout << "PASSED in " << double(clock() - c0) / CLOCKS_PER_SEC << "s\n";
  }

}
