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

class OpExpression : public Expression {
  Expression * lhs;
  Expression * rhs;

 public:
  OpExpression(Expression * l, Expression * r) : lhs(l), rhs(r) {}
  std::string getOpString(std::string Op) const {
    std::string s = "(" + lhs->toString() + Op + rhs->toString() + ")";
    return s;
  }
  ~OpExpression() {
    delete lhs;
    delete rhs;
  }
};

class PlusExpression : public OpExpression {
 public:
  PlusExpression(Expression * l, Expression * r) : OpExpression(l, r) {}
  virtual std::string toString() const { return getOpString(" + "); }
};

class MinusExpression : public OpExpression {
 public:
  MinusExpression(Expression * l, Expression * r) : OpExpression(l, r) {}
  virtual std::string toString() const { return getOpString(" - "); }
};

class TimesExpression : public OpExpression {
 public:
  TimesExpression(Expression * l, Expression * r) : OpExpression(l, r) {}
  virtual std::string toString() const { return getOpString(" * "); }
};

class DivExpression : public OpExpression {
 public:
  DivExpression(Expression * l, Expression * r) : OpExpression(l, r) {}
  virtual std::string toString() const { return getOpString(" / "); }
};

#endif
