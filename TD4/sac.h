#ifndef __SAC_H
#define __SAC_H

#include <vector>
using namespace std;

// Description d'un objet qu'on peut mettre dans le sac à dos.
struct Item {
  int weight;    // Poids
  double value;  // Valeur.
};

// Détermine le choix optimal d'objets à placer dans un sac à dos de
// capacité C pour maximiser leur valeur.
// Renvoie cette valeur maximale, et écrit la liste des objets choisis (un même
// objet ne peut être pris qu'une fois!), dans un ordre quelquonque, dans le
// vecteur "chosen_objects".
//
// IMPORTANT: Le test de cette fonction est en 2 parties: d'abord, il teste
// simplement la valeur de renvoi, ensuite il teste "chosen_objects". Si vous
// ne remplissez pas ce dernier, vous pourrez quand meme avoir des points.
double Knapsack(int C, const vector<Item>& items,
                vector<int>* chosen_items)


#endif  // __SAC_H
