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

 public:
  OperationExpression(Expression * lhs, Expression * rhs) :
      lOperand(lhs),
      rOperand(rhs) {}
  virtual std::string toString() const = 0;
  virtual ~OperationExpression() {
    delete lOperand;
    delete rOperand;
  }
};

class PlusExpression : public OperationExpression {
 public:
  PlusExpression(Expression * lhs, Expression * rhs) : OperationExpression(lhs, rhs) {}
  std::string toString() const {
    std::stringstream ss;
    ss << "(" << lOperand->toString() << " + " << rOperand->toString() << ")";
    return ss.str();
  }
  virtual ~PlusExpression() {}
};
