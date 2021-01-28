#include "tree.2.h"
int Height(const vector<vector<int>>& enfants, int node){
  if (enfants[node].empty()){
    return 0;
  }else{
    int maxDepth = 0;
    for (int v : enfants[node]){
      int tmpIndex = Height(enfants, v);
      //in case Height(enfants, 3) = 2 for node 3 we have only two children whose they do not have children, so the longest sub-tree is 3--2--1
      if (tmpIndex>maxDepth){
        maxDepth = tmpIndex;
      }
    }
    return maxDepth+1;
  }
}
