#include "graph.h"
#include "toposort.h"
#include "main_utils.h"
#include <algorithm>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main() {
  {
    cout << "Trivial examples." << endl;
    DirectedGraph g(3);
    CHECK_EQ(ToStr(CanonicalizeCopy(TopologicalSort(g))), "[0 1 2]");
    cout << "PASSED at line " << __LINE__ << endl;

    g.AddArc(0, 2);
    g.AddArc(2, 1);
    CHECK_EQ(ToStr(TopologicalSort(g)), "[1 2 0]");
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Testing with cycles" << endl;
    DirectedGraph g(3);
    g.AddArc(1, 0);
    g.AddArc(0, 2);
    g.AddArc(2, 1);
    CHECK_EQ(ToStr(TopologicalSort(g)), "[]");
    cout << "PASSED at line " << __LINE__ << endl;

    DirectedGraph g2(5);
    g2.AddArc(1, 0);
    g2.AddArc(0, 2);
    g2.AddArc(2, 1);
    g2.AddArc(1, 3);
    g2.AddArc(4, 2);
    CHECK_EQ(ToStr(TopologicalSort(g2)), "[]");
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Small example." << endl;
    DirectedGraph g(8);
    g.AddArc(3, 1);
    g.AddArc(1, 5);
    g.AddArc(2, 5);
    g.AddArc(5, 6);
    g.AddArc(6, 7);
    g.AddArc(7, 4);
    g.AddArc(6, 0);
    vector<int> order = TopologicalSort(g);
    CHECK_EQ(order.size(), g.NumNodes());
    // Canonicalize the order.
    sort(order.begin(), order.begin() + 3);
    sort(order.begin() + 5, order.begin() + 8);
    CHECK_EQ(ToStr(order), "[0 4 7 6 5 1 2 3]");
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Randomized test." << endl;
    const clock_t c0 = clock();
    const int trials = 100;
    const int repeat = 10;
    Random random(123);
    for (int i = 0; i < trials; ++i) {
      const int N = random.Uniform(1000) + 1;
      const int M = N * random.Uniform(std::min(10, N-1));
      // Generate a random topological order, then build a big DAG out of it,
      // then re-compute the topological order (which might be slightly different)
      // and verify that it fits the DAG.
      vector<int> nodes(N, 0);
      std::iota(nodes.begin(), nodes.end(), 0);
      shuffle(nodes.begin(), nodes.end(), random);
      DirectedGraph g(N);
      for (int i = 0; i < M; ++i) {
        const int a = random.Uniform(N-1);
        const int b = random.Uniform(N-a-1) + a + 1;
        g.AddArc(a, b);
      }
      for (int i = 0; i < repeat; ++i) {
        const vector<int> order = TopologicalSort(g);
        if (i != 0) continue;
        CHECK_EQ(order.size(), N);
        vector<int> rank(N, -1);
        for (int r = 0; r < N; ++r) {
          CHECK_GE(order[r], 0);
          CHECK_LT(order[r], N);
          CHECK_EQ(rank[order[r]], -1);  // No repetitions.
          rank[order[r]] = r;
        }
        for (int a = 0; a < N; ++a) {
          for (int b : g.Neighbors(a)) {
            CHECK_GT(rank[a], rank[b]);
          }
        }
      }
    }
    cout << "PASSED at line " << __LINE__ << endl;
    cout << "Time: " << double(clock() - c0) / CLOCKS_PER_SEC << "s\n";
  }
}
