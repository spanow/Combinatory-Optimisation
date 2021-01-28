#include "flow2.h"
#include "ortools/linear_solver/linear_solver.h"

#include <string>

using namespace operations_research;

vector<FlowOnArc> BestFlow2(int num_nodes, const vector<BackboneArc>& backbone,
                            const vector<FlowDemand>& demands,
                            double max_free_utilization_ratio,
                            double penalty_cost) {
  // On commence comme pour l'exercice précédent...
  MPSolver solver("Mon solveur Backbone-2", MPSolver::GLOP_LINEAR_PROGRAMMING);
  const int num_arcs = backbone.size();
  const int num_demands = demands.size();
  const double infinity = solver.infinity();
  MPObjective* objective = solver.MutableObjective();
  objective->SetMaximization();

  // On définit les variables de flot, indexées par demande et par arc.
  vector<vector<MPVariable*>> flow_var(num_demands);
  for (int d = 0; d < num_demands; ++d) {
    for (int a = 0; a < num_arcs; ++a) {
      flow_var[d].push_back(
          solver.MakeNumVar(0, infinity,
                            "Flow(demand:" + std::to_string(d) +
                                ",arc:" + std::to_string(a) + ")"));
    }
  }

  // Contraintes de capacité.
  /*for (int a = 0; a < num_arcs; ++a) {
    MPConstraint* ct = solver.MakeRowConstraint(
        0, backbone[a].capacity, "ArcCapacity" + std::to_string(a));
    for (int d = 0; d < num_demands; ++d) {
      ct->SetCoefficient(flow_var[d][a], 1.0);
    }
  }*/

  // Ré-adaptation des contraintes de capacity selon les variables de "sur-utilisation".
  //(example de solution donné en cours Lundi - revoir mes notes).
  for (int a = 0; a < num_arcs; ++a) {
    MPVariable* var = solver.MakeNumVar(0, 1.0 - max_free_utilization_ratio,"MaxUse #"+std::to_string(a));
    // redefinition de la nouvelle objectif
    objective->SetCoefficient(var, -penalty_cost);
    MPConstraint* ct = solver.MakeRowConstraint(0, backbone[a].capacity * max_free_utilization_ratio,"Use #"+std::to_string(a));
    ct->SetCoefficient(var, -backbone[a].capacity);
    for (int d = 0; d < num_demands; ++d) {
      ct->SetCoefficient(flow_var[d][a], 1.0);
    }
  }

  // Pour faire les contraintes de flot, on pré-calcule les listes d'adjacence.
  vector<vector<int>> out_arc_indices(num_nodes);
  vector<vector<int>> in_arc_indices(num_nodes);
  for (int a = 0; a < num_arcs; ++a) {
    out_arc_indices[backbone[a].from].push_back(a);
    in_arc_indices[backbone[a].to].push_back(a);
  }

  // Contraintes de flot: pour chaque demande D, a chaque noeud:
  // -OutMax <= flot sortant - flot entrant <= InMax
  // où InMax vaut D.demand pour le noeud D.src et 0 sinon, et
  // OutMax vaut D.demand pour le noeud D.dst et 0 sinon.
  for (int d = 0; d < num_demands; ++d) {
    for (int node = 0; node < num_nodes; ++node) {
      MPConstraint* ct = solver.MakeRowConstraint(
          node == demands[d].dst ? -demands[d].demand : 0.0,  // Borne inf
          node == demands[d].src ? demands[d].demand : 0.0,   // Borne sup
          "Flow(demand:" + std::to_string(d) + ",node:" + std::to_string(node));
      for (int a : out_arc_indices[node]) {
        ct->SetCoefficient(flow_var[d][a], 1.0);
      }
      for (int a : in_arc_indices[node]) {
        ct->SetCoefficient(flow_var[d][a], -1.0);
      }
    }
  }

  // On définit l'objectif, qui est de maximiser le prix de vente du
  // flot: on va le mesurer sur les "noeuds d'entrée" (et donc sur les
  // arc d'entrée) de chaque demande.
  // ATTENTION il faut aussi compter négativement les arcs entrants.
  for (int d = 0; d < num_demands; ++d) {
    for (int a : out_arc_indices[demands[d].src]) {
      objective->SetCoefficient(flow_var[d][a], demands[d].price);
    }
    for (int a : in_arc_indices[demands[d].src]) {
      objective->SetCoefficient(flow_var[d][a], -demands[d].price);
    }
  }

  // C'est fini! On résout et on renvoie la solution.
  solver.Solve();
  vector<FlowOnArc> flow_on_arc;
  for (int d = 0; d < num_demands; ++d) {
    for (int a = 0; a < num_arcs; ++a) {
      const double flow = flow_var[d][a]->solution_value();
      // Mini optimisation: on inclut pas les variables de flow nul.
      if (flow == 0.0) continue;
      flow_on_arc.push_back({a, d, flow});
    }
  }
  return flow_on_arc;
}
