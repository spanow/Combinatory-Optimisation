#include "job.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include "ortools/linear_solver/linear_solver.h"

using namespace operations_research;
using namespace std;

// Cette fonction nous aide a extraire la resource numero #n:
// 0->cpu, 1->ram, 2->disk.
double NthResource(const Resources& r, int n) {
  return n == 0 ? r.cpu : n == 1 ? r.ram : r.disk;
}

vector<int> BestJobAssignment(const vector<Resources>& jobs,
                              const vector<Resources>& machines) {
  MPSolver solver("Mon solveur", MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);
  const int num_jobs = jobs.size();
  const int num_machines = machines.size();

  MPObjective* objective = solver.MutableObjective();
  objective->SetMaximization();

  // On definit les variables: var[i][j]=1 si on fait tourner le job #i sur la
  // machine #j.
  vector<vector<MPVariable*>> var(num_jobs);
  for (int i = 0; i < num_jobs; ++i){
    for (int j = 0; j < num_machines; ++j) {
      MPVariable* v = solver.MakeBoolVar("Job" + std::to_string(i) + "On" +
                                         std::to_string(j));
      var[i].push_back(v);
      // Contribution de cet ingrédient au prix total (l'objectif).
      objective->SetCoefficient(v, 1);
    }
  }

  // Chaque job tourne au plus une fois.
  for (int i = 0; i < num_jobs; ++i) {
    MPConstraint* ct = solver.MakeRowConstraint(0, 1, "");  // TODO: name
    for (int j = 0; j < num_machines; ++j) {
      ct->SetCoefficient(var[i][j], 1.0);
    }
  }

  // Les jobs tournant sur une machine ne doivent pas consommer plus de
  // resources que la machine! On fait la meme chose pour chaque type de
  // resource.
  for (int j = 0; j < num_machines; ++j) {
    for (int res = 0; res < 3; ++res) {
      MPConstraint* ct =  // TODO: name
          solver.MakeRowConstraint(0, NthResource(machines[j], res), "");
      for (int i = 0; i < num_jobs; ++i) {
        ct->SetCoefficient(var[i][j], NthResource(jobs[i], res));
      }
    }
  }

  // On résout! Le problème devrait admettre une solution optimale
  // trouvable sans trop de problème.
  solver.EnableOutput();
  solver.Solve();

  // On renvoie la solution.
  vector<int> assignment(num_jobs, -1);
  for (int i = 0; i < num_jobs; ++i) {
    for (int j = 0; j < num_machines; ++j) {
      if (var[i][j]->solution_value() > 0.5) {
        if (assignment[i] != -1) {
          cerr << "BUG: double assignment, Job #" << i << ", machines #"
               << assignment[i] << " and #" << j << endl;
          exit(1);
        }
        assignment[i] = j;
      }
    }
  }
  return assignment;
}
