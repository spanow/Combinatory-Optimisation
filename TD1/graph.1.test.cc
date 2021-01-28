#include "graph.h"
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
    DirectedGraph g(5);
    g.AddArc(1, 0);
    g.AddArc(1, 3);
    g.AddArc(3, 1);
    g.AddArc(2, 0);
    g.AddArc(1, 2);
    g.AddArc(3, 4);
    g.AddArc(3, 4);
    CHECK_EQ(5, g.NumNodes());
    CHECK_EQ(0, g.OutDegree(0));
    CHECK_EQ(3, g.OutDegree(1));
    CHECK_EQ(1, g.OutDegree(2));
    CHECK_EQ(3, g.OutDegree(3));
    CHECK_EQ(0, g.OutDegree(4));
    CHECK_EQ("[]", ToStr(CanonicalizeCopy(g.Neighbors(0))));
    CHECK_EQ("[0 2 3]", ToStr(CanonicalizeCopy(g.Neighbors(1))));
    CHECK_EQ("[0]", ToStr(CanonicalizeCopy(g.Neighbors(2))));
    CHECK_EQ("[1 4 4]", ToStr(CanonicalizeCopy(g.Neighbors(3))));
    CHECK_EQ("[]", ToStr(CanonicalizeCopy(g.Neighbors(4))));
    cout << "SUCCESS!\n";
  }

  {
    cout << "Test #" << num_tests++ << ": empty graph." << endl;
    // Empty graph
    DirectedGraph g(0);
    CHECK_EQ(0, g.NumNodes());
    cout << "SUCCESS!\n";
  }

  {
    cout << "Test #" << num_tests++ << ": performance." << endl;
    clock_t c0 = clock();
    const int num_nodes = 50000;
    DirectedGraph g(num_nodes);
    CHECK_EQ(g.NumNodes(), num_nodes);
    const int avg_deg = 50;
    const int num_arcs = num_nodes * avg_deg;
    for (int i = 0; i < num_arcs; ++i) {
      g.AddArc(rand() % num_nodes, rand() % num_nodes);
    }
    int sum = 0;
    for (int i = 0; i < num_nodes; ++i) {
      CHECK_EQ(g.Neighbors(i).size(), g.OutDegree(i));
      sum += g.OutDegree(i);
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
