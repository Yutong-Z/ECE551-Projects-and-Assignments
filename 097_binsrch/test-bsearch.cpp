#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "function.h"

class CountedIntFn : public Function<int, int> {
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const char * mesg;

 public:
  CountedIntFn(unsigned n, Function<int, int> * fn, const char * m) :
      remaining(n),  // maximum number of invocations allowed
      f(fn),         // actual function to comptue with
      mesg(m)        // a message which is useful to know what happened if the test fails
  {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr, "Too many function invocations in %s\n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
};

int binarySearchForZero(Function<int, int> * f, int low, int high);

// Computed the maximum number of invocations of f
// Wrap f in a CountedIntFn
// Perform the binary search
// And check the answer
// mesg: function name tested
void check(Function<int, int> * f,
           int low,
           int high,
           int expected_ans,
           const char * mesg) {
  int n = log2(high - low) + 1;
  Function<int, int> * fWrap = new CountedIntFn(n, f, mesg);
  int ans = binarySearchForZero(fWrap, low, high);
  if (expected_ans != ans) {
    fprintf(stderr, "Wrong answer got by binarySearchForZero with function %s\n", mesg);
    exit(EXIT_FAILURE);
  }
  delete fWrap;
}

class SinFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 10000000 * (sin(arg / 100000.0) - 0.5); }
};

class LogFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return log2(100000 + arg); }
};

int main(void) {
  const char * mesg;

  Function<int, int> * f1 = new SinFunction;
  mesg = "SinFunction";
  check(f1, 0, 150000, 52359, mesg);
  // check(f1, 52360, 150000, 52360, mesg);  // f(x) is all positive
  // check(f1, 0, 52350, 52350, mesg);       // f(x) is all negative
  check(f1, 0, 52359, 52358, mesg);
  check(f1, 52359, 150000, 52359, mesg);
  delete f1;

  Function<int, int> * f2 = new LogFunction;
  mesg = "LogFunction";
  check(f2, -200000, 150000, 1 - 100000, mesg);

  delete f2;

  return EXIT_SUCCESS;
}
