#ifndef __EXPR_H__
#define __EXPR_H__

#include <cstdlib>
#include <sstream>
#include <string>

class Expression {
 public:
  virtual std::string toString() const = 0;
  virtual ~Expression() {}
};

class NumExpression : public Expression {
  long num;

 public:
  NumExpression(long n) : num(n) {}
  virtual std::string toString() const {
    std::stringstream ss;
    ss << num;
    std::string s = ss.str();
    return s;
  }
};

class PlusExpression : public Expression {
  Expression * lhs;
  Expression * rhs;

 public:
  PlusExpression(Expression * l, Expression * r) : lhs(l), rhs(r) {}
  virtual std::string toString() const {
    std::string s = "(" + lhs->toString() + " + " + rhs->toString() + ")";
    return s;
  }
  ~PlusExpression() {
    delete lhs;
    delete rhs;
  }
};

#endif
