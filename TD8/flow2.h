#ifndef __FLOW2_H
#define __FLOW2_H

#include "flow.h"

// Come BestFlow() (voir flow.h), mais on ajoute une penalité pour trop
// "remplir" les arcs du backbone: si le taux d'utilisation U d'un arc
// (entre 0 et 1, 0 = pas utilisé, 1 = utilisé à sa limite de capacité)
// est supérieur à x, on paiera une penalité proportionelle à (U-x).
//
// On veut toujours maximiser le gain global, mais en prenant en compte
// la penalité de ces arcs trop remplis.
//
// Exemple:
// - Un backbone avec 4 arcs de capacités 1000, 300, 500, 100.
// - Une solution ou la somme des flots sur ces arcs vaut: 840, 300, 470, 90.
// Si max_free_utilization_ratio=0.9, les arcs en "sur-utilisation" sont les
// 2eme et 3eme arcs. Le 1er est en-dessous du seuil, le dernier est juste au
// seuil. Les coefficients de sur-utilisations des arcs sont: 0, 0.1, 0.04, 0.
// Si penalt_cost=100, le coût de cette sur-utilisation est de:
// 0.1*penalty_cost + 0.04*penalty_cost.
//
// INDICATIONS:
// On pourra imiter et meme remplacer les contraintes de capacités en
// introduisant des variables de "sur-utilisation", et en adaptant la contrainte
// de capacité avec ces variables.
vector<FlowOnArc> BestFlow2(int num_nodes, const vector<BackboneArc>& backbone,
                            const vector<FlowDemand>& demands,
                            double max_free_utilization_ratio,
                            double penalty_cost);

#endif  // __FLOW2_H
