#include "tree.1.h"

vector<vector<int>> ParentVectorToChildrenVector(const vector<int>& parent){
  vector<vector<int>> children(parent.size());
  for (int i = 0; i < parent.size(); i++) {
      if (parent[i] != -1){
        children[parent[i]].push_back(i);
      }
  }
  return children;
}
