#include "bfs.3.h"
#include <algorithm>

vector<int> GetShortestPathFromRootedTree(const vector<int>& parent, int target) {
  vector<int> path;
  if (parent[target] == -1) return path;
  while (true) {
    path.push_back(target);
    int new_target = parent[target];
    if (new_target == target) break;
    target = new_target;
  }
  std::reverse(path.begin(), path.end());
  return path;
}
