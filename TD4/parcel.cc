#include "parcel.h"

int MaxGainAux(const vector<int>& g, int i){
  if (i >= g.size()){
    return 0;
  }else{

  }
}

int MaxGain(const vector<int>& gain){

  return MaxGainAux(gain);
}

for (int i = 0; i < gain.size(); i ++){
  if (i == 0){
    return gain[i] + = MaxGain(gain[i+2]);
  }else{
    return gain[i+1] = MaxGain(gain[i+1]);
  }
}
