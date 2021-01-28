#include "sac1.h"
#include "algorithm"

int Sac1(int C, const vector<int>& p){
  vector<int>copieP;
  copieP = p ;
  int maxObj = 0;
  int i = 0;
  int allWeight = 0;
  int maxw = C;
  sort(copieP.begin(), copieP.end());
  for(int i = 0; i<copieP.size(); i++){
    allWeight += copieP[i];
  }
  while ((maxw >= 0) && (i >= 0)){
    if (allWeight < maxw){
      return copieP.size();
    }
    maxw -= copieP[i];
    if (maxw < 0){
      break;
    }else{
      i++;
      maxObj++;
    }
  }
  return maxObj;
}
