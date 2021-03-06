#include "flow2.h"

#include "main_utils.h"
using namespace std;

struct TestCase {
  string name;
  vector<BackboneArc> arcs;
  vector<FlowDemand> demands;
  double max_free_utilization;
  double penalty_cost;
  double gain_optimal;
};

int NumNodes(const vector<BackboneArc>& arcs) {
  int max_node = -1;
  for (const BackboneArc& arc : arcs) {
    max_node = max(max_node, arc.from);
    max_node = max(max_node, arc.to);
  }
  return max_node + 1;
}

ostream& operator<<(ostream& out, const FlowOnArc& f) {
  return out << "flow[d=" << f.demand_index << "][arc="
             << f.arc_index << "]=" << f.flow;
}

int main() {
  cout.precision(12);
  cerr.precision(12);
  vector<TestCase> test_cases = {
    {"TrivialNetworkFullOverUtilized",
     {{0, 1, 10.0}},
     {{0, 1, 20.0, 5.0}},
     0.8,
     1.0,
     49.8,
    },
    {"TrivialNetworkNotOverUtilized",
     {{0, 1, 10.0}},
     {{0, 1, 20.0, 5.0}},
     0.8,
     100.0,
     40.0,
    },
    {"TrivialDemandFull1",
     {{0, 1, 20.0}},
     {{0, 1, 10.0, 5.0}},
     0.8,
     1.0,
     50.0,
    },
    {"TrivialDemandFull2",
     {{0, 1, 20.0}},
     {{0, 1, 10.0, 5.0}},
     0.5,
     1000.0,
     50.0,
    },
    {"TrivialZeroCapacity",
     {{0, 1, 0.0}},
     {{0, 1, 20.0, 5.0}},
     1.0,
     0.0,
     0.0,
    },
    {"TrivialZeroDemand",
     {{0, 1, 20.0}},
     {{0, 1, 0.0, 5.0}},
     1.0,
     0.0,
     0.0,
    },
    {"TrivialNoPath",
     {{0, 1, 20.0}},
     {{1, 0, 10.0, 5.0}},
     1.0,
     0.0,
     0.0,
    },
    // A small example that can be solved optimally by applying a greedy
    // strategy.
    // Nodes are 0, 1, 2, 4 (3 is disconnected).
    // Arc capacities are in [.]
    // 
    //  .-->0<---[10]--->1   Demands: 0->4, max=10, price = 2.0
    //  |   |            |            1->4, max=7, price = 3.0
    // [4] [8]           |            1->2, max=9, price = 4.0
    //  |   |           [5]           2->1, max=9, price = 5.0
    //  2<--'            | 
    //  |                |   Optimal solution:
    //  '--[6]--->4<-----'   - demand (2->1) gets flow=4 -> gain=20
    //                       - demand (1->2) gets flow=8 -> gain=32
    //                       - demand (1->4) gets flow=5 -> gain=15
    //                       - demand (0->4) gets no flow.
    {"SmallGreedyNoOverUtilizationConstraint",
     {{0, 2, 8.0},
      {2, 0, 4.0},
      {2, 4, 6.0},
      {1, 4, 5.0},
      {0, 1, 10.0},
      {1, 0, 10.0},
     },
     {{0, 4, 10.0, 2.0},
      {1, 4, 7.0, 3.0},
      {1, 2, 9.0, 4.0},
      {2, 1, 9.0, 5.0},
     },
     1.0,
     1000.0,
     20. + 32. + 15.,
    },
    // Same, with various over-utilization scenarios.
    {"SmallGreedyNoOverUtilizationConstraint2",
     {{0, 2, 8.0},
      {2, 0, 4.0},
      {2, 4, 6.0},
      {1, 4, 5.0},
      {0, 1, 10.0},
      {1, 0, 10.0},
     },
     {{0, 4, 10.0, 2.0},
      {1, 4, 7.0, 3.0},
      {1, 2, 9.0, 4.0},
      {2, 1, 9.0, 5.0},
     },
     0.3,
     0.0,
     20. + 32. + 15.,
    },
    {"SmallGreedyOverUtilization",
     {{0, 2, 8.0},
      {2, 0, 4.0},
      {2, 4, 6.0},
      {1, 4, 5.0},
      {0, 1, 10.0},
      {1, 0, 10.0},
     },
     {{0, 4, 10.0, 2.0},
      {1, 4, 7.0, 3.0},
      {1, 2, 9.0, 4.0},
      {2, 1, 9.0, 5.0},
     },
     0.9,
     100.0,
     60.3,
    },
  };
  // Add a big, randomized test case.
  Random random(123);
  const int n_nodes = 50;
  const int n_arcs = 300;
  const int n_demands = 50;
  TestCase big;
  big.name = "BigRandomized";
  while (big.arcs.size() < n_arcs) {
    const int from = random.Uniform(n_nodes);
    const int to = random.Uniform(n_nodes);
    if (from == to) continue;
    big.arcs.push_back({from, to, random.RandDouble()});
  }
  while (big.demands.size() < n_demands) {
    const int from = random.Uniform(n_nodes);
    const int to = random.Uniform(n_nodes);
    if (from == to) continue;
    big.demands.push_back({from, to, random.RandDouble(), random.RandDouble()});
  }
  big.gain_optimal = 14.367331116;
  big.max_free_utilization = 0.9;
  big.penalty_cost = 1.0;
  test_cases.push_back(big);

  // Another one!
  big.name += "2";
  big.penalty_cost = 0.1;
  big.gain_optimal = 14.5004605969;
  test_cases.push_back(big);

  // And another one.
  big.name.back() = '3';
  big.penalty_cost = 0.01;
  big.gain_optimal = 14.7354561743;
  test_cases.push_back(big);

  for (const TestCase& t : test_cases) {
    cout << endl << "=========================================================="
         << endl << "Testing '" << t.name << "'..." << endl;
    const int num_nodes = NumNodes(t.arcs);
    const vector<FlowOnArc> sol = BestFlow2(num_nodes, t.arcs, t.demands, t.max_free_utilization, t.penalty_cost);
    // Verify that it's a solution!
    vector<vector<double>> out_per_node_per_demand(num_nodes, vector<double>(t.demands.size(), 0));
    vector<vector<double>> in_per_node_per_demand = out_per_node_per_demand;
    vector<double> flow_per_arc(t.arcs.size(), 0);
    for (const FlowOnArc& f : sol) {
      CHECK_GE(f.arc_index, 0);
      CHECK_LT(f.arc_index, t.arcs.size());
      CHECK_GE(f.demand_index, 0);
      CHECK_LT(f.demand_index, t.demands.size());
      CHECK_GE(f.flow, -1e-6);
      CHECK_LT(f.flow, t.arcs[f.arc_index].capacity + 1e-6);
      flow_per_arc[f.arc_index] += f.flow;
      out_per_node_per_demand[t.arcs[f.arc_index].from][f.demand_index] += f.flow;
      in_per_node_per_demand[t.arcs[f.arc_index].to][f.demand_index] += f.flow;
    }
    double gain = 0.0;
    for (int a = 0; a < t.arcs.size(); ++a) {
      if (flow_per_arc[a] > t.arcs[a].capacity + 1e-6) {
        cerr << "ERROR: the aggregated flow on arc #" << a << " ("
             << t.arcs[a].from << "->" << t.arcs[a].to << ") is "
             << flow_per_arc[a] << ", which is over its capacity "
             << t.arcs[a].capacity << endl;
        cerr << "Printing your solution:" << endl << sol << endl;
        return 1;
      }
      const double usage = flow_per_arc[a] / t.arcs[a].capacity;
      if (usage > t.max_free_utilization) {
        gain -= t.penalty_cost * (usage - t.max_free_utilization);
      }
    }
    for (int d = 0; d < t.demands.size(); ++d) {
      for (int node = 0; node < num_nodes; ++node) {
        double sum = out_per_node_per_demand[node][d] - in_per_node_per_demand[node][d];
        double ub = 0;
        double lb = 0;
        if (node == t.demands[d].src) {
          gain += sum * t.demands[d].price;
          ub = t.demands[d].demand;
        }
        if (node == t.demands[d].dst) {
          lb = -t.demands[d].demand;
        }
        if (sum < lb - 1e-6 || sum > ub + 1e-6) {
          cerr << "ERROR: the aggregated flow on node #" << node
               << " for demand #" << d << " is " << sum
               << " which is out of the allowed bounds ["
               << lb << " .. " << ub << "]" << endl;
          cerr << "Printing your solution:" << endl;
          cerr << sol << endl;
          return 1;
        }
      }
    }
    // Remove the penalty
    if (gain < t.gain_optimal - 1e-6) {
      cerr << "ERROR: Feasible solution, but not optimal! (" << gain 
           << ", optimal:" << t.gain_optimal << ")" << endl;
      cerr << "Printing your solution:" << endl << sol << endl;
      return 1;
    } else {
      cout << "PASSED: You found an optimal solution!" << endl;
    }
  }
}
