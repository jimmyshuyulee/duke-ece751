#include <sstream>
#include <string>

class Expression {
 public:
  Expression() {}
  virtual std::string toString() const = 0;
  virtual ~Expression() {}
};

class NumExpression : public Expression {
 protected:
  long number;

 public:
  NumExpression(long num) : number(num) {}
  std::string toString() const {
    std::stringstream ss;
    ss << number;
    return ss.str();
  }
  virtual ~NumExpression() {}
};

class OperationExpression : public Expression {
 protected:
  Expression * lOperand;
  Expression * rOperand;
  std::string oper;

 public:
  OperationExpression(Expression * lhs, Expression * rhs, std::string op) :
      lOperand(lhs),
      rOperand(rhs),
      oper(op) {}
  virtual std::string toString() const {
    std::stringstream ss;
    ss << "(" << lOperand->toString() << " " << oper << " " << rOperand->toString()
       << ")";
    return ss.str();
  }
  virtual ~OperationExpression() {
    delete lOperand;
    delete rOperand;
  }
};

class PlusExpression : public OperationExpression {
 public:
  PlusExpression(Expression * lhs, Expression * rhs) :
      OperationExpression(lhs, rhs, "+") {}
  //  std::string toString() const {}
  virtual ~PlusExpression() {}
};

class MinusExpression : public OperationExpression {
 public:
  MinusExpression(Expression * lhs, Expression * rhs) :
      OperationExpression(lhs, rhs, "-") {}
  //  std::string toString() const {}
  virtual ~MinusExpression() {}
};
class TimesExpression : public OperationExpression {
 public:
  TimesExpression(Expression * lhs, Expression * rhs) :
      OperationExpression(lhs, rhs, "*") {}
  //  std::string toString() const {}
  virtual ~TimesExpression() {}
};
class DivExpression : public OperationExpression {
 public:
  DivExpression(Expression * lhs, Expression * rhs) :
      OperationExpression(lhs, rhs, "/") {}
  //  std::string toString() const {}
  virtual ~DivExpression() {}
};
