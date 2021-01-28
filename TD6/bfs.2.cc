#include "bfs.2.h"

vector<int> GetBfsDistances(const vector<int>& parent) {
  const int n = parent.size();
  vector<int> distance(n, -1);
  for (int i = 0; i < n; ++i) {
    if (parent[i] == -1) continue;
    int p = i;
    int dist = 0;
    while (parent[p] != p && distance[p] == -1) {
      dist++;
      p = parent[p];
    }
    if (parent[p] == p) distance[p] = 0;
    dist += distance[p];
    p = i;
    while (parent[p] != p && distance[p] == -1) {
      distance[p] = dist;
      dist--;
    }
  }
  return distance;
}
