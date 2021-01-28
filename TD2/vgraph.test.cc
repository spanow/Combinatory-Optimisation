#include "vgraph.h"
#include "main_utils.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main() {
  DirectedGraph g(5);
  CHECK_EQ(5, g.NumNodes());

  g.AddArc(1, 0, 4.5);
  g.AddArc(1, 3, 3.2);
  g.AddArc(3, 1, 0.0);
  g.AddArc(2, 0, 0.1);
  g.AddArc(1, 2, 7.7);
  g.AddArc(3, 4, 8.8);
  g.AddArc(3, 4, 0.9);
  CHECK_EQ(0, g.OutDegree(0));
  CHECK_EQ(3, g.OutDegree(1));
  CHECK_EQ(1, g.OutDegree(2));
  CHECK_EQ(3, g.OutDegree(3));
  CHECK_EQ(0, g.OutDegree(4));
  CHECK_EQ("[]", ToStr(CanonicalizeCopy(g.Neighbors(0))));
  CHECK_EQ("[(0, 4.5) (2, 7.7) (3, 3.2)]", ToStr(CanonicalizeCopy(g.Neighbors(1))));
  CHECK_EQ("[(0, 0.1)]", ToStr(CanonicalizeCopy(g.Neighbors(2))));
  CHECK_EQ("[(1, 0) (4, 0.9) (4, 8.8)]", ToStr(CanonicalizeCopy(g.Neighbors(3))));
  CHECK_EQ("[]", ToStr(CanonicalizeCopy(g.Neighbors(4))));

  cout << "SUCCESS!\n";
}
