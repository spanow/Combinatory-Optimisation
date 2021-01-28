#include "tree.4.h"
int SubtreeSizeAux(const vector<vector<int>>& enfants, int node, vector<int>& memoisation) {
  if (memoisation[node] >= 0) return memoisation[node];
  int total = 1;
  for (int e : enfants[node]) {
    total += SubtreeSizeAux(enfants, e, memoisation);
  }
  return memoisation[node] = total;
}

vector<int> AllSubtreeSizes(const vector<vector<int>>& enfants){
  const int nb_nodes = enfants.size();
  vector<int> sum(nb_nodes, -1);
  for (int node = 0; node < nb_nodes; node++){
    if (enfants[node].empty()){
      sum[node] = 1;
    }else{
      SubtreeSizeAux(enfants, node, sum);
    }
  }
  return sum;
}
