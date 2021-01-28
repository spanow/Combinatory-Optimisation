#ifndef __JOB_H
#define __JOB_H

#include <vector>

using std::vector;

// On veut affecter des tâches (job) à des machines.
// On a un ensemble de machines hétérogènes, et des descriptions de jobs.
// Chaque job utilise une quantité prédéfinie de CPU, RAM et disque de la
// machine sur laquelle il tournera, et chaque machine a une quantité
// prédéfinie des CPU, RAM et disque.
//
// On veut maximiser le nombre de jobs affectés.
//
// Cette fonction doit renvoyer un assignment optimal des jobs à chaque
// machine: l'élement #i sera le numero de machine à laquelle le job #i
// sera affecté, où -1 si on décide de ne pas affecter le job.
struct Resources {
  double cpu;
  double ram;
  double disk;
};
vector<int> BestJobAssignment(const vector<Resources>& jobs,
                              const vector<Resources>& machines);

#endif  // __JOB_H
