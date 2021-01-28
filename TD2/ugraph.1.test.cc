#include "ugraph.h"
#include "main_utils.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main() {
  int num_tests = 0;
  // Build the example in the .h
  {
    cout << "Test #" << num_tests++ << ": basic example." << endl;
    UndirectedGraph g(5);
    g.AddEdge(1, 0);
    g.AddEdge(1, 3);
    g.AddEdge(2, 0);
    g.AddEdge(1, 2);
    g.AddEdge(4, 3);
    g.AddEdge(3, 4);
    CHECK_EQ(g.NumNodes(), 5);
    CHECK_EQ(g.NumEdges(), 6);
    CHECK_EQ(g.Degree(0), 2);
    CHECK_EQ(g.Degree(1), 3);
    CHECK_EQ(g.Degree(2), 2);
    CHECK_EQ(g.Degree(3), 3);
    CHECK_EQ(g.Degree(4), 2);
    CHECK_EQ("[1 2]", ToStr(CanonicalizeCopy(g.Neighbors(0))));
    CHECK_EQ("[0 2 3]", ToStr(CanonicalizeCopy(g.Neighbors(1))));
    CHECK_EQ("[0 1]", ToStr(CanonicalizeCopy(g.Neighbors(2))));
    CHECK_EQ("[1 4 4]", ToStr(CanonicalizeCopy(g.Neighbors(3))));
    CHECK_EQ("[3 3]", ToStr(CanonicalizeCopy(g.Neighbors(4))));
    cout << "SUCCESS!\n";
  }

  {
    cout << "Test #" << num_tests++ << ": empty graph." << endl;
    // Empty graph
    UndirectedGraph g(0);
    CHECK_EQ(0, g.NumNodes());
    CHECK_EQ(0, g.NumEdges());
    cout << "SUCCESS!\n";
  }

  {
    cout << "Test #" << num_tests++ << ": performance." << endl;
    clock_t c0 = clock();
    const int num_nodes = 100000;
    UndirectedGraph g(num_nodes);
    CHECK_EQ(g.NumNodes(), num_nodes);
    const int avg_deg = 50;
    const int num_arcs = num_nodes * avg_deg;
    for (int i = 0; i < num_arcs; ++i) {
      g.AddEdge(rand() % num_nodes, rand() % num_nodes);
    }
    int sum = 0;
    for (int i = 0; i < num_nodes; ++i) {
      CHECK_EQ(g.Neighbors(i).size(), g.Degree(i));
      sum += g.Degree(i);
    }
    const double t = double(clock() - c0) / CLOCKS_PER_SEC;
    cout << "Time: " << t << "s." << endl;
    if (t < 1) {
      cout << "SUCCESS!\n";
    } else {
      cout << "Too slow!\n";
    }
  }
}
