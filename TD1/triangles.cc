#include "triangles.h"
#include "main_utils.h"
#include <algorithm>
using namespace std;

// a method: for each pair of connected nodes, find if they have a
// common neighbor
int NumTriangles(const UndirectedGraph& graph){
  int num_nodes = graph.NumNodes();
  int count = 0;
  vector<int> vec1;
  vector<int> vec2;

  // first, we build a sorted adj list. This is tedious! We should do this
  // in the graph class directly.
  vector<vector<int> > sorted_adj;
  for (int n1 = 0; n1 < num_nodes; n1++) {
    vector<int> vec1 = graph.Neighbors(n1);
    sort(vec1.begin(), vec1.end());
    sorted_adj.push_back(vec1);
  }

  for (int n1 = 0; n1 < num_nodes; n1++) {
    vec1 = sorted_adj[n1];
  }
  // iterate on connected pairs n1, n2
  // first n1
  for (int n1 = 0; n1 < num_nodes; n1++) {
    vec1 = sorted_adj[n1];
    int s1 = vec1.size();

    // then a connected n2
    for (int ind = 0; ind < s1; ind++) {
      int n2 = vec1[ind];
      vec2 = sorted_adj[n2];
      int s2 = vec2.size();

      int it1 = 0;
      int it2 = 0;
      // go through the (sorted) lists and find common elements
      while (it1 < s1 and it2 < s2) {
        while (vec1[it1] < vec2[it2] and it1 < s1) {
          it1++;
        }
        if (it1 == s1) {
          break;
        }
        if (vec1[it1] == vec2[it2]) {
          count++;
        }
        it2++;
        if (it2 == s2) {
          break;
        }
      }
    }
  }
  // each triangle has been counted 6 times
  return count / 6;

}
