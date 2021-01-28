#include "tree.3.h"
#include <iostream>
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
   int res = 0;
    if (enfants[node].empty()){
      return 0;
    }else{
      for (int v : enfants[node]){
        res = HeightsWithMem(v, enfants, GroupByLengthByChild);
        GroupByLengthByChild[node] = res;
      }
      return res; // return H for a given node.
    }
}
