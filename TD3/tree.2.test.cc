#include "tree.2.h"
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
    CHECK_EQ(Height({{1}, {2}, {}}, 0), 2);
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(Height({{}, {0}, {1}, {2}, {3}}, 3), 3);
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(Height({{}, {}, {0, 3, 1, 4}, {}, {}}, 2), 1);
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Small tree example." << endl;
    const vector<vector<int>> tree = {{}, {}, {1}, {2, 0, 4}, {}};
    CHECK_EQ(Height(tree, 0), 0);
    CHECK_EQ(Height(tree, 1), 0);
    CHECK_EQ(Height(tree, 2), 1);
    CHECK_EQ(Height(tree, 3), 2);
    CHECK_EQ(Height(tree, 4), 0);
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Forest (disconnected trees) example." << endl;
    const vector<vector<int>> forest = {
      {1, 2}, {3, 4}, {5}, {}, {}, {6}, {}, {8}, {10}, {}, {9}};
    CHECK_EQ(Height(forest, 0), 3);
    CHECK_EQ(Height(forest, 1), 1);
    CHECK_EQ(Height(forest, 2), 2);
    CHECK_EQ(Height(forest, 3), 0);
    CHECK_EQ(Height(forest, 4), 0);
    CHECK_EQ(Height(forest, 5), 1);
    CHECK_EQ(Height(forest, 6), 0);
    CHECK_EQ(Height(forest, 7), 3);
    CHECK_EQ(Height(forest, 8), 2);
    CHECK_EQ(Height(forest, 9), 0);
    CHECK_EQ(Height(forest, 10), 1);
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Randomized performance test" << endl;
    const clock_t c0 = clock();
    const int trials = 100;
    const int repeat = 1000;
    Random random(123);
    for (int i = 0; i < trials; ++i) {
      const int N = random.Uniform(1000) + 1;
      const int H = N == 1 ? 0 : random.Uniform(N - 1) + 1;
      // Generate a random tree of height H.
      vector<int> nodes(N, 0);
      vector<int> height(N, 0);
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
        for (int i = cur; i < cur + num; ++i) {
          height[nodes[i]] = h;
        }
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
      for (int i = 0; i < repeat; ++i) {
        const int node = random.Uniform(N);
        const int h = Height(children, node);
        CHECK_EQ(h, height[node]);
      }
    }
    cout << "PASSED at line " << __LINE__ << endl;
    cout << "Time: " << double(clock() - c0) / CLOCKS_PER_SEC << "s\n";
  }
  {
    cout << "Corner case: one node" << endl;
    CHECK_EQ(Height({{}}, 0), 0);
    cout << "PASSED at line " << __LINE__ << endl;
  }
}
