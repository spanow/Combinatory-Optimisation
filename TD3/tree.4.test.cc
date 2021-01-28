#include "tree.4.h"
#include "main_utils.h"
#include <algorithm>
#include <numeric>
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>
using namespace std;

int main() {
  {
    cout << "Trivial trees example." << endl;
    CHECK_EQ(ToStr(AllSubtreeSizes({{1}, {2}, {}})), "[3 2 1]");
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(ToStr(AllSubtreeSizes({{}, {0}, {1}, {2}, {3}})), "[1 2 3 4 5]");
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(ToStr(AllSubtreeSizes({{}, {}, {0, 3, 1, 4}, {}, {}})), "[1 1 5 1 1]");
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Small tree example." << endl;
    const vector<vector<int>> tree = {{}, {}, {1}, {2, 0, 4}, {}};
    CHECK_EQ(ToStr(AllSubtreeSizes(tree)), "[1 1 2 5 1]");
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Forest (disconnected trees) example." << endl;
    const vector<vector<int>> forest = {
      {1, 2}, {3, 4}, {5}, {}, {}, {6}, {}, {8}, {10}, {}, {9}};
    CHECK_EQ(ToStr(AllSubtreeSizes(forest)), "[7 3 3 1 1 2 1 4 3 1 2]");
    cout << "PASSED at line " << __LINE__ << endl;
  }
  {
    cout << "Randomized performance test" << endl;
    const clock_t c0 = clock();
    const int trials = 100;
    const int repeat = 10;
    Random random(123);
    for (int i = 0; i < trials; ++i) {
      const int N = random.Uniform(1000) + 1;
      const int H = N == 1 ? 0 : random.Uniform(N - 1) + 1;
      // Generate a random tree of height H.
      vector<int> nodes(N, 0);
      vector<vector<int>> children(N);
      unordered_set<int> noparent;
      std::iota(nodes.begin(), nodes.end(), 0);
      shuffle(nodes.begin(), nodes.end(), random);
      int cur = 0;
      int lastnum = nodes.size();
      vector<int> tmp;
      for (int h = 0; h <= H; ++h) {
        const int num_left = nodes.size() - cur;
        if (h == H) CHECK_EQ(num_left, 1);
        const int hleft = H - h;
        const int mini = hleft == 0 ? 1 : (num_left + hleft - 2) / hleft;
        const int maxi = std::min(lastnum, num_left - hleft);
        CHECK_GE(maxi, mini);
        const int num = random.Uniform(maxi - mini + 1) + mini;
        if (h > 0) {
          const int last = cur - num;
          for (int i = 0; i < num; ++i) {
            children[nodes[cur + i]].push_back(nodes[last + i]);
            noparent.erase(nodes[last + i]);
          }
          for (int i : noparent) if (random.Uniform(hleft + 1) == 0) {
            tmp.push_back(i);
          }
          for (int i : tmp) {
            children[nodes[random.Uniform(num) + cur]].push_back(i);
            noparent.erase(i);
          }
        }
        for (int i = cur; i < cur + num; ++i) {
          noparent.insert(nodes[i]);
        }
        tmp.clear();
        lastnum = num;
        cur += num;
      }
      CHECK_EQ(noparent.size(), 1);  // The root.
      vector<int> sizes(N, 1);
      // They're in topo order so we're good.
      for (int node : nodes) {
        for (int child : children[node]) {
          sizes[node] += sizes[child];
        }
      }
      for (int i = 0; i < repeat; ++i) {
        CHECK_EQ(AllSubtreeSizes(children), sizes);
      }
    }
    cout << "PASSED at line " << __LINE__ << endl;
    cout << "Time: " << double(clock() - c0) / CLOCKS_PER_SEC << "s\n";
  }

  {
    cout << "Corner case: one node" << endl;
    CHECK_EQ(AllSubtreeSizes({{}}), vector<int>{1});
    cout << "PASSED at line " << __LINE__ << endl;
  }
  {
    cout << "Corner case: zero node" << endl;
    CHECK_EQ(AllSubtreeSizes({}), vector<int>());
    cout << "PASSED at line " << __LINE__ << endl;
  }
}
