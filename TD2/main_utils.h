#ifndef __MAIN_UTILS_H
#define __MAIN_UTILS_H

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

typedef long long int64;

/* To use if mt19937 proves to be platform-dependent. BEWARE! Less "random" than mt19937.
class Random {
 public:
  explicit Random(int seed) {
    seed_ = seed & 0x7fffffffu;  // Make it non-negative.
    if (seed_ == 0 || seed_ == 0x7fffffffu) seed_ = 1;  // Prevent forbidden values.
  }

  inline int RandInt() {
    unsigned long long p = seed_ * 16807ULL;  // bits 14, 8, 7, 5, 2, 1, 0
    seed_ = (unsigned int)((p >> 31) + (p & 0x7fffffffu));
    if (seed_ > 0x7fffffffu) seed_ -= 0x7fffffffu;
    return seed_;
  }

  inline double RandDouble() { return RandInt() / double(0x7fffffff); }

  inline int Uniform(int max) { return max == 0 ? 0 : RandInt() % max; }

  // To support shuffle(x.begin(), x.end(), Random());
  typedef int result_type;
  int min() const { return 0; }
  int max() const { return INT_MAX; }
  inline int operator()() { return RandInt(); }

 private:
  unsigned int seed_;
};
*/

class Random : public std::mt19937 {
 public:
  explicit Random(int seed) : std::mt19937(seed) {}

  inline int RandInt() { return operator()(); }
  inline double RandDouble() { return operator()() / (max() + 1.0); }
  inline double RandExp() {
    return -log1p(-RandDouble());
  }  // in [0, +inf), with mean=1
  inline int Uniform(int max) { return operator()() % max; }
};

// We re-implement std::shuffle because its algorithm may vary across
// platforms/compilers, thus affecting the random number generator.
// This version is meant to be deterministic.
template <class It, class Random>
void shuffle(It begin, It end, Random& rng) {
  if (begin == end) return;
  while (true) {
    const int dist = end - begin;
    if (dist == 1) return;
    std::swap(*begin, *(begin + rng() % dist));
    ++begin;
  }
}

#define CHECK(x)                                              \
  do {                                                        \
    if (!(x)) {                                               \
      cerr << "ERROR in " __FILE__ ":" << __LINE__            \
           << ": CHECK condition violated: " #x << std::endl; \
      exit(1);                                                \
    }                                                         \
  } while (0)

#define CHECK_BINARY(test, x, y)                                              \
  do {                                                                        \
    auto X = x;                                                               \
    auto Y = y;                                                               \
    if (!(X test Y)) {                                                        \
      cerr << "ERROR in " __FILE__ ":" << __LINE__                            \
           << ": CHECK violated: \"" #x " " #test " " #y "\" (values: '" << X \
           << "' and '" << Y << "')" << std::endl;                            \
      exit(1);                                                                \
    }                                                                         \
  } while (0)

#define CHECK_EQ(x, y) CHECK_BINARY(==, x, y)
#define CHECK_NE(x, y) CHECK_BINARY(!=, x, y)
#define CHECK_LT(x, y) CHECK_BINARY(<, x, y)
#define CHECK_GT(x, y) CHECK_BINARY(>, x, y)
#define CHECK_LE(x, y) CHECK_BINARY(<=, x, y)
#define CHECK_GE(x, y) CHECK_BINARY(>=, x, y)

#define CHECK_NEAR(x, y, eps)                                              \
  do {                                                                     \
    double xx = (x);                                                       \
    double yy = (y);                                                       \
    if (xx != yy && fabs(xx - yy) / (fabs(xx) + fabs(yy)) > double(eps)) { \
      cerr << "ERROR in " __FILE__ ":" << __LINE__                         \
           << ": CHECK_NEAR violated: \"" #x " ~= " #y                     \
           << " with relative precision " << #eps << "\" (values: " << xx  \
           << " ~= " << yy << ")" << std::endl;                            \
      exit(1);                                                             \
    }                                                                      \
  } while (0)

template <class T>
string ToStr(const T& t);

template <class U, class V>
ostream& operator<<(ostream& o, const pair<U, V>& p);

template <class T>
ostream& operator<<(ostream& out, const vector<T>& v);

template <typename T>
string StrJoin(const T& v, const string& sep);

template <class U, class V>
ostream& operator<<(ostream& o, const pair<U, V>& p) {
  return o << "(" << p.first << ", " << p.second << ")";
}

template <typename T>
string StrJoin(const T& v, const string& sep) {
  stringstream ss;
  int size = 0;
  for (const auto& x : v) {
    if (size++) ss << sep;
    ss << x;
  }
  return ss.str();
}

template <class T>
ostream& operator<<(ostream& out, const vector<T>& v) {
  return out << '[' << StrJoin(v, " ") << ']';
}

template <class T>
string ToStr(const T& t) {
  stringstream ss;
  ss << t;
  return ss.str();
}

template <class T>
void Canonicalize(T*) {}

template <class T>
void Canonicalize(vector<T>* v) {
  for (T& x : *v) Canonicalize(&x);
  sort(v->begin(), v->end());
}

template <class T>
T CanonicalizeCopy(const T& t) {
  T copy(t);
  Canonicalize(&copy);
  return copy;
}

template <class T>
vector<T> CanonicalizeElements(const vector<T>& v) {
  vector<T> out;
  out.reserve(v.size());
  for (const T& x : v) out.push_back(CanonicalizeCopy(x));
  return out;
}

#endif  // __MAIN_UTILS_H
