#include <sstream>
#include <string>

class Expression {
 public:
  Expression() {}
  virtual std::string toString() const = 0;
  virtual long evaluate() const = 0;
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
  virtual long evaluate() const { return number; };
  virtual ~NumExpression() {}
};

class OperationExpression : public Expression {
 protected:
  Expression * lOperand;
  Expression * rOperand;
  std::string oper;

  OperationExpression(Expression * lhs, Expression * rhs, std::string op) :
      lOperand(lhs),
      rOperand(rhs),
      oper(op) {}

 public:
  virtual std::string toString() const {
    std::stringstream ss;
    ss << "(" << lOperand->toString() << " " << oper << " " << rOperand->toString()
       << ")";
    return ss.str();
  }
  virtual long evaluate() const = 0;
  virtual ~OperationExpression() {
    delete lOperand;
    delete rOperand;
  }
};

class PlusExpression : public OperationExpression {
 public:
  PlusExpression(Expression * lhs, Expression * rhs) :
      OperationExpression(lhs, rhs, "+") {}
  virtual long evaluate() const { return lOperand->evaluate() + rOperand->evaluate(); };
  virtual ~PlusExpression() {}
};

class MinusExpression : public OperationExpression {
 public:
  MinusExpression(Expression * lhs, Expression * rhs) :
      OperationExpression(lhs, rhs, "-") {}
  virtual long evaluate() const { return lOperand->evaluate() - rOperand->evaluate(); };
  virtual ~MinusExpression() {}
};
class TimesExpression : public OperationExpression {
 public:
  TimesExpression(Expression * lhs, Expression * rhs) :
      OperationExpression(lhs, rhs, "*") {}
  virtual long evaluate() const { return lOperand->evaluate() * rOperand->evaluate(); };
  virtual ~TimesExpression() {}
};
class DivExpression : public OperationExpression {
 public:
  DivExpression(Expression * lhs, Expression * rhs) :
      OperationExpression(lhs, rhs, "/") {}
  virtual long evaluate() const { return lOperand->evaluate() / rOperand->evaluate(); };
  virtual ~DivExpression() {}
};
