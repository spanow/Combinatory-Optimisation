#include "tree.1.h"
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

    CHECK_EQ("[[]]", ToStr(ParentVectorToChildrenVector({-1})));
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ("[[] [0] [1] [2]]",
             ToStr(ParentVectorToChildrenVector({1, 2, 3, -1})));
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ("[[] [0] [1] [2]]",
             ToStr(ParentVectorToChildrenVector({1, 2, 3, -1})));
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "More complex example." << endl;
    CHECK_EQ("[[] [5] [1] [0 2 4] [] []]",
             ToStr(CanonicalizeElements(ParentVectorToChildrenVector(
                   {3, 2, 3, -1, 3, 1}))));
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Forest (disconnected trees) example." << endl;
    CHECK_EQ("[[2 5] [0] [] [6] [3] [7] [] [] []]",
             ToStr(CanonicalizeElements(ParentVectorToChildrenVector(
                   {1, -1, 0, 4, -1, 0, 3, 5, -1}))));
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Randomized performance test" << endl;
    const clock_t c0 = clock();
    const int trials = 100;
    const int repeat = 1;
    Random random(123);
    for (int i = 0; i < trials; ++i) {
      const int N = random.Uniform(10000) + 1;
      vector<int> nodes(N, 0);
      std::iota(nodes.begin(), nodes.end(), 0);
      shuffle(nodes.begin(), nodes.end(), random);
      vector<int> parent(N, -1);
      for (int i = 1; i < N; ++i) parent[nodes[i]] = nodes[random.Uniform(i)];
      const vector<vector<int>> children = ParentVectorToChildrenVector(parent);
      // Check for correctness
      CHECK_EQ(children.size(), N);
      for (int p = 0; p < N; ++p) {
        for (int c : children[p]) {
          CHECK_GE(c, 0);
          CHECK_LT(c, N);
          CHECK_EQ(parent[c], p);
        }
      }
      // Check that it's always the same return value.
      for (int i = 1; i < repeat; ++i) {
        CHECK_EQ(ParentVectorToChildrenVector(parent), children);
      }
    }
    cout << "PASSED at line " << __LINE__ << endl;
    cout << "Time: " << double(clock() - c0) / CLOCKS_PER_SEC << "s\n";
  }
  {
    cout << "Corner case: zero nodes" << endl;
    CHECK_EQ("[]", ToStr(ParentVectorToChildrenVector({})));
    cout << "PASSED at line " << __LINE__ << endl;
  }
}
