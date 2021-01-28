#include "job3.h"

#include <algorithm>
#include <set>
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
  vector<vector<int>> exclu;
  vector<pair<int, int>> dep;
  int max_jobs;
};

int main() {
  cout.precision(12);
  vector<TestCase> test_cases;
  // A few randomized test cases of increasing sizes.
  Random random(123);
  const int num_tests = 10;
  const int kExpected[] = {2, 2, 3, 2, 7, 9, 16, 16, 19, 23};
  int num_jobs = 0;
  int num_machines = 0;
  const double kFact = 2;
  const double kMaxCpu = 32.0;
  const double kMaxRam = 128.0;     // GB
  const double kMaxDisk = 10000.0;  // GB
  for (int test = 0; test < num_tests; ++test) {
    num_jobs += random.Uniform(9);
    num_machines += random.Uniform(4);
    TestCase big;
    big.name = "Random" + std::to_string(test) + "With" +
               std::to_string(num_jobs) + "Jobs" +
               std::to_string(num_machines) + "Machines";
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
    // exclu
    const int num_exclu = num_jobs < 2 ? 0 : random.Uniform(num_jobs);
    while (big.exclu.size() < num_exclu) {
      const int size_exclu = random.RandExp();
      //cout << size_exclu << endl;
      if (size_exclu < 2 ||
          size_exclu > std::max(std::min(num_jobs, 3), num_jobs / 2))
        continue;
      vector<int> all_jobs;
      for (int i = 0; i < num_jobs; ++i) all_jobs.push_back(i);
      shuffle(all_jobs.begin(), all_jobs.end(), Random(34));
      all_jobs.resize(size_exclu);
      big.exclu.push_back(std::move(all_jobs));
    }
    // dep
    // const int num_deps = random.Uniform(num_jobs);
    const int num_deps = random.Uniform(1);
    while (big.dep.size() < num_deps) {
      const int a = random.Uniform(num_jobs);
      const int b = random.Uniform(num_jobs);
      if (a == b) continue;
      big.dep.push_back({a, b});
    }

    big.max_jobs = kExpected[test];

    test_cases.push_back(big);
  }

  for (const TestCase& t : test_cases) {
    cout << endl
         << "==========================================================" << endl
         << "Testing '" << t.name << "'..." << endl;
    const vector<int> sol = BestJobAssignment3(t.jobs, t.machines, t.exclu);
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
             << " is over-subscribed: assigned jobs need " << res[m]
             << ", machine has " << t.machines[m] << endl;
        cerr << "Printing your solution:" << endl << sol << endl;
        return 1;
      }
    }
    // Verify that the exclusivity constraints are verified.
    for (int i = 0; i < t.exclu.size(); ++i) {
      const vector<int>& ex = t.exclu[i];
      for (int a : ex) {
        for (int b : ex) {
          if (a >= b) continue;
          if (sol[a] != -1 && sol[b] != -1) {
            cerr << "ERROR: In your assignment, jobs #" << a << " and #" << b
                 << " are both assigned,"
                 << " despite being listed as exclusive"
                 << " (exclusivity constraint #" << i << ")," << endl;
            return 1;
          }
        }
      }
    }

    if (num_assigned < t.max_jobs - 1) {
      cerr << "ERROR: Suboptimal solution! You assigned " << num_assigned
           << " jobs, you could have assigned " << t.max_jobs << endl;
      cerr << "Printing your solution:" << endl << sol << endl;
      return 1;
    }
    if (num_assigned > t.max_jobs) {
      cerr << "WARNING: Your solution is better than a supposedly 'optimal' "
              "known solution,"
           << " which indicates a problem with the solver. Please report this!"
           << " Your solution: " << num_assigned
           << ", previously supposed 'optimal' solution: " << t.max_jobs
           << endl;
    }
    cout << "PASSED! You found an optimal solution" << endl;
  }
}
