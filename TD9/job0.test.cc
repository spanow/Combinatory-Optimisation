#include "job0.h"

#include <algorithm>
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
  vector<double> profits;
  vector<Resources> machines;
  double ref;
  double ref2;
};

int main() {
  cout.precision(12);
  vector<TestCase> test_cases;
  // A few randomized test cases of increasing sizes.
  Random random(123);
  const int num_tests = 10;
  const vector<double> kRef = {38.6545, 55.12,  92.02,  105.22, 149.70,
                               248.31,  346.77, 366.89, 486.63, 617.99};
  const vector<double> kRef2 = {38.6606, 55.1215, 93.0152, 106.197, 177.767,
                                342.872, 356.61,  366.891, 572.762, 917.701};
  int num_jobs = 50;
  int num_machines = 15;
  const double kFact = 4;
  const double kMaxCpu = 32.0;
  const double kMaxRam = 128.0;     // GB
  const double kMaxDisk = 10000.0;  // GB
  for (int test = 0; test < num_tests; ++test) {
    num_jobs = num_jobs * (random.RandDouble() + 1);
    num_machines = num_machines * (random.RandDouble() + 1);
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
      big.profits.push_back(random.RandDouble());
    }
    for (int i = 0; i < num_machines; ++i) {
      Resources machine;
      machine.cpu = kFact * random.RandDouble() * kMaxCpu;
      machine.ram = kFact * random.RandDouble() * kMaxRam;
      machine.disk = kFact * random.RandDouble() * kMaxDisk;
      big.machines.push_back(machine);
    }
    big.ref = kRef[test];
    big.ref2 = kRef2[test];
    test_cases.push_back(big);
  }

  for (const TestCase& t : test_cases) {
    cout << endl
         << "==========================================================" << endl
         << "Testing '" << t.name << "'..." << endl;
    const clock_t c0 = clock();
    const vector<int> sol = BestJobAssignment0(t.jobs, t.profits, t.machines);
    const double tt = double(clock() - c0) / CLOCKS_PER_SEC;
    if (tt > 10) {
      cerr
          << "WARNING: Your program took " << tt
          << "s which is more than 10s! It may be scored zero in the real test!"
          << endl;
    }
    // Verify that it's a solution!
    const int num_jobs = t.jobs.size();
    const int num_machines = t.machines.size();
    CHECK_EQ(sol.size(), num_jobs);
    // Verify that each machine has enough resources.
    vector<Resources> res(num_machines, Resources{0, 0, 0});
    double profit = 0.0;
    for (int i = 0; i < num_jobs; ++i) {
      if (sol[i] < 0) continue;
      profit += t.profits[i];
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
    double delta = std::max(1.01, t.ref2 / t.ref);
    double score =
        0.1 *
        int(std::min(100.0, 30 * pow(1.5, log(profit / t.ref) / log(delta))));
    if (tt > 11) score = 0;
    cout << "SCORE: " << score << " (reference: [" << t.ref << ".." << t.ref2
         << "], yours: " << profit << ")" << endl;
  }
}
