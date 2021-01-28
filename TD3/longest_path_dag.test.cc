#include "graph.h"
#include "longest_path_dag.h"
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
    vector<int> poids = {0, 1, 2};
    CHECK_EQ(LongestPathInDag(g, poids), 2);
    cout << "PASSED at line " << __LINE__ << endl;

    g.AddArc(0, 2);
    g.AddArc(2, 1);
    CHECK_EQ(LongestPathInDag(g, poids), 3);
    cout << "PASSED at line " << __LINE__ << endl;

    g.AddArc(1, 0);
    CHECK_EQ(LongestPathInDag(g, poids), -1);
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
    g.AddArc(4, 7);
    g.AddArc(6, 0);
    vector<int> poids = {4, 1, 4, 2, 11, 5, 1, 2};
    CHECK_EQ(LongestPathInDag(g, poids), 14);
    cout << "PASSED at line " << __LINE__ << endl;
  }
  {
    cout << "Randomized test." << endl;
    Random random(123);
    const int N = 10000;
    const int M = N * 10;
    // Generate a random topological order, then build a big DAG out of it.
    vector<int> nodes(N, 0);
    std::iota(nodes.begin(), nodes.end(), 0);
    shuffle(nodes.begin(), nodes.end(), random);
    DirectedGraph g(N);
    for (int i = 0; i < M; ++i) {
      const int a = random.Uniform(N-1);
      const int b = random.Uniform(N-a-1) + a + 1;
      g.AddArc(a, b);
    }
    vector<int> poids(N, 0);
    for (int& p : poids) {
      p = random.Uniform(1000);
    }
    const clock_t c0 = clock();
    CHECK_EQ(LongestPathInDag(g, poids), 68396);
    cout << "PASSED at line " << __LINE__ << endl;
    cout << "Time: " << double(clock() - c0) / CLOCKS_PER_SEC << "s\n";
  }
}
