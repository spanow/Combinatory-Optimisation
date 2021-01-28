#include "job.h"

#include "main_utils.h"
using namespace std;

Resources operator+(const Resources& a, const Resources& b) {
  return {a.cpu + b.cpu, a.ram + b.ram, a.disk + b.disk};
}

bool operator<=(const Resources& a, const Resources& b) {
  return a.cpu <= b.cpu && a.ram <= b.ram && a.disk <= b.disk;
}

ostream& operator<<(ostream& out, const Resources& r) {
  return out << "CPU=" << r.cpu << ",RAM=" << r.ram << ",DISK=" << r.disk;
}

struct TestCase {
  string name;
  vector<Resources> jobs;
  vector<Resources> machines;
  int max_jobs;
};

int main() {
  cout.precision(12);
  vector<TestCase> test_cases = {
    // Single-machine tests
    {"OneJobEasyFit",
     {{0.1, 0.2, 0.3}},
     {{0.2, 0.4, 0.6}},
     1,
    },
    {"AllJobsFit",
     {{0.1, 0.2, 0.3}, {1.0, 2.0, 3.0}},
     {{9.0, 9.0, 9.0}},
     2,
    },
    {"NoFit",
     {{0.11, 0.22, 0.33}},
     {{0.1, 0.2, 0.3}},
     0,
    },
    {"TwoOutOfThreeJobPerfectFit",
     {{0.1, 0.2, 0.3},{0.1, 0.2, 0.3},{0.1, 0.2, 0.3}},
     {{0.2, 0.4, 0.6}},
     2,
    },
    {"CpuBound3Of5",
     {{0.1, 0.2, 0.3},{0.12, 0.24, 0.36},{10.0, 0.0, 1.0},{1.0, 10.0, 0.0},{0.0, 1.0, 10.0}},
     {{1.2, 99, 99}},
     3,
    },
    {"RamBound3Of5",
     {{0.1, 0.2, 0.3},{0.12, 0.24, 0.36},{10.0, 0.0, 1.0},{1.0, 10.0, 0.0},{0.0, 1.0, 10.0}},
     {{99, 1.4, 99}},
     3,
    },
    {"DiskBound3Of5",
     {{0.1, 0.2, 0.3},{0.12, 0.24, 0.36},{10.0, 0.0, 1.0},{1.0, 10.0, 0.0},{0.0, 1.0, 10.0}},
     {{99, 99, 1.6}},
     3,
    },
    // Multi-machine tests
    {"OneJobManyMachines",
     {{1., 1., 1.}},
     {{2., 2., 2.}, {3., 3., 3.}, {4., 4., 4.}},
     1,
    },
    {"OneJobManyMachinesOnlyOneFits",
     {{1., 1., 1.}},
     {{1., 1., .5}, {.5, 1., 1.}, {1., 1., 1.}, {1., .5, 1.}},
     1,
    },
    {"FourJobsFourMachinesPerfectMatch",
     {{1., 1., 1.}, {.5, 1., 1.}, {1., 1., .5}, {1., .5, 1.}},
     {{1., 1., .5}, {.5, 1., 1.}, {1., 1., 1.}, {1., .5, 1.}},
     4,
    },
    {"FitsInNoMachine",
     {{1., 1., 1.}},
     {{.9, 2., 2.}, {2., 2., .9}, {2., .9, 2.}},
     0,
    },
    {"SixJobsTwoMachinesCanFit5",
     {{1., .2, 3.}, {.3, .4, .1}, {.4, .1, 2.}, {2., .5, 0}, {0., 2., 0.}, {2., 0., 1.}},
     {{1.41, .31, 5.1}, {4.1, 2.51, 1.1}},
     5,
    },
    {"SixJobsTwoMachinesCanFit5DespiteSumGreater",
     {{1., .2, 3.}, {.3, .4, .1}, {.4, .1, 2.}, {2., .5, 0}, {0., 2., 0.}, {2., 0., 1.}},
     {{1.421, .351, 6.001}, {4.201, 2.551, 2.001}},
     5,
    },
  };
  // Add a big, randomized test case.
  Random random(123);
  const int num_jobs = 50;
  const int num_machines = 20;
  const double kFact = 2;
  const double kMaxCpu = 32.0;
  const double kMaxRam = 128.0;  // GB
  const double kMaxDisk = 10000.0;  // GB
  TestCase big;
  big.name = "BigRandomized";
  for (int i = 0; i < num_jobs; ++i) {
    Resources job;
    job.cpu = random.RandDouble() * kMaxCpu;
    job.ram = random.RandDouble() * kMaxRam;
    job.disk = random.RandDouble() * kMaxDisk;
    big.jobs.push_back(job);
  }
  for (int i = 0; i < num_machines; ++i) {
    Resources machine;
    machine.cpu = kFact * random.RandDouble() * kMaxCpu;
    machine.ram = kFact * random.RandDouble() * kMaxRam;
    machine.disk = kFact * random.RandDouble() * kMaxDisk;
    big.machines.push_back(machine);
  }
  big.max_jobs = 32;
  test_cases.push_back(big);

  for (const TestCase& t : test_cases) {
    cout << endl << "=========================================================="
         << endl << "Testing '" << t.name << "'..." << endl;
    const vector<int> sol = BestJobAssignment(t.jobs, t.machines);
    // Verify that it's a solution!
    const int num_jobs = t.jobs.size();
    const int num_machines = t.machines.size();
    CHECK_EQ(sol.size(), num_jobs);
    // Verify that each machine has enough resources.
    vector<Resources> res(num_machines, Resources{0, 0, 0});
    int num_assigned = 0;
    for (int i = 0; i < num_jobs; ++i) {
      if (sol[i] < 0) continue;
      ++num_assigned;
      CHECK_GE(sol[i], 0);
      CHECK_LT(sol[i], num_machines);
      res[sol[i]] = res[sol[i]] + t.jobs[i];
    }
    for (int m = 0; m < num_machines; ++m) {
      if (res[m] <= t.machines[m]) {
        // OK
      } else {
        cerr << "ERROR: In your assignment, machine #" << m
             << " is over-subscribed: assigned jobs need "
             << res[m] << ", machine has " << t.machines[m] << endl;
        cerr << "Printing your solution:" << endl << sol << endl;
        return 1;
      }
    }
    if (num_assigned != t.max_jobs) {
      cerr << "ERROR: Suboptimal solution! You assigned "
           << num_assigned << " jobs, you could have assigned "
           << t.max_jobs << endl;
      cerr << "Printing your solution:" << endl << sol << endl;
      return 1;
    }
    cout << "PASSED! You found an optimal solution" << endl;
  }
}
