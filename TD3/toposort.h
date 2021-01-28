#ifndef __TOPOSORT_H
#define __TOPOSORT_H

#include "graph.h"
#include <vector>
using namespace std;

// Fait le tri topologique du graphe dirigé donné en argument, et renvoie
// les indices des noeuds triés selon l'ordre topologique: les "feuilles"
// (noeuds n'ayant pas d'arcs sortant) seront plutot vers le début.
// Si le graphe a un cycle (ce n'est donc pas un DAG, il n'y a donc pas
// d'ordre topologique), la fonction devra renvoyer une tableau vide.
//
// A NOTER / INDICES:
// - Plusieurs ordres topologiques différents existent souvent.
// - On pourra précalculer la liste des parents de chaque noeuds (car on
//   ne dispose que de la liste des enfants via le graphe).
// - On pourra pré-calculer le "degré sortant résiduel" de chaque noeud,
//   et initialiser une file FIFO (queue<int> par exemple) qui contiendra
//   tous les noeuds de degré residuel zéro, mise à jour dynamiquement.
// - La complexité devra être O(M) (nombre d'arcs).
//
// IMPORTANT!!! Dans le cours je donnais l'autre convention de valeurs de
// sortie. Ici on renvoie les noeuds triés par ordre topologique. C'est plus
// facile: au lieu de faire (comme dans le cours):
//    int num = 0;                          // Initialisation
//    vector<int> numero(num_nodes, -1);    // Initialisation
//    ...
//    numero[node] = num++;                 // On 'traite' le noeud "node".
//    ...
//    return numero;
// Vous aurez plutot quelquechose de ce genre dans le TD:
//    vector<int> tri;      // Initialisation
//    ...
//    tri.push_back(node);  // On 'traite' le noeud "node"
//    ...
//    return tri;
//
// Exemple:
//
// 2-->5-->6-->7   Plusieurs ordres topologiques valides (liste non exhaustive):
//     ^   |   |    - [4, 0, 7, 6, 5, 1, 2, 3]
//     |   v   v    - [0, 4, 7, 6, 5, 2, 1, 3]
// 3-->1   0   4    - [4, 7, 0, 6, 5, 1, 3, 2]
//
vector<int> TopologicalSort(const DirectedGraph& graph);

#endif  // __TOPOSORT_H
