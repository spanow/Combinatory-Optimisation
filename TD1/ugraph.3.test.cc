#include "ugraph.h"
#include "main_utils.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main() {
  int num_tests = 0;
  {
    cout << "Test #" << num_tests++ << ": basic example." << endl;
    UndirectedGraph g(10);
    g.AddEdge(1, 0);
    g.AddEdge(1, 3);
    g.AddEdge(2, 0);
    g.AddEdge(1, 2);
    g.AddEdge(7, 3);
    g.AddEdge(3, 9);
    g.AddEdge(5, 8);
    g.AddEdge(5, 8);
    g.AddEdge(5, 8);
    g.AddEdge(8, 5);
    g.AddEdge(5, 8);
    g.AddEdge(5, 8);
    // Connected components: (4), (6), (5 8) and (everything else)
    CHECK_EQ("[[0 1 2 3 7 9] [4] [5 8] [6]]",
             ToStr(CanonicalizeCopy(g.ConnectedComponents())));
    // Try again. If it fails there, then the code isn't cleaning up
    // its internal data structures properly.
    CHECK_EQ("[[0 1 2 3 7 9] [4] [5 8] [6]]",
             ToStr(CanonicalizeCopy(g.ConnectedComponents())));
    cout << "SUCCESS!\n";
  }

  {
    cout << "Test #" << num_tests++ << ": performance." << endl;
    const int num_nodes = 1500;
    UndirectedGraph g(num_nodes);
    CHECK_EQ(g.NumNodes(), num_nodes);
    const int num_edges = num_nodes * 0.55;
    for (int i = 0; i < num_edges; ++i) {
      g.AddEdge(rand() % num_nodes, rand() % num_nodes);
    }
    clock_t c0 = clock();
    int sum = 0;
    for (int i = 0; i < num_nodes; ++i) {
      sum += g.ConnectedComponents().size();
    }
    const double t = double(clock() - c0) / CLOCKS_PER_SEC;
    cout << "Time: " << t << "s." << endl;
    CHECK_GT(sum, pow(num_nodes, 1+2.0/3));
    CHECK_LT(sum, pow(num_nodes, 2.0));
    if (t < 1) {
      cout << "SUCCESS!\n";
    } else {
      cout << "Too slow!\n";
    }
  }
}
