#include "tree.3.h"
//AllHights is same as Hights but for all sub-trees.
vector<int> AllHeights(const vector<vector<int>>& enfants){
  int allNodes = enfants.size();
  vector<int> GroupByLengthByChild(allNodes, -1); //Init vector with value -1 because we don't know the distance yet, as we did in BFS.
  for (int i = 0; i < allNodes; i++){ // i does represent the node itself.
      GroupByLengthByChild[i]=HeightsWithMem(i, enfants, GroupByLengthByChild);
  }
  return GroupByLengthByChild;
}

 int HeightsWithMem(int node, const vector<vector<int>>& enfants, vector<int>& GroupByLengthByChild){
   if(GroupByLengthByChild[node]>=0){
     return GroupByLengthByChild[node];
   }
    if (enfants[node].empty()){
      return 0;
    }
      int res = 0;
      for (int v : enfants[node]){
        res = max(res,HeightsWithMem(v, enfants, GroupByLengthByChild));
      }
      return GroupByLengthByChild[node] = res + 1; // return H for a given node.
}
