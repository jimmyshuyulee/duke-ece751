#include "IntMatrix.h"

IntMatrix::IntMatrix() : numRows(0), numColumns(0), rows(new IntArray *[0]) {
}
IntMatrix::IntMatrix(int r, int c) : numRows(r), numColumns(c), rows(new IntArray *[r]) {
  for (int i = 0; i < r; i++) {
    rows[i] = new IntArray(c);
  }
}
IntMatrix::IntMatrix(const IntMatrix & rhs) :
    numRows(rhs.numRows),
    numColumns(rhs.numColumns),
    rows(new IntArray *[rhs.numRows]) {
  for (int i = 0; i < rhs.numRows; i++) {
    rows[i] = new IntArray(*rhs.rows[i]);
  }
}
IntMatrix::~IntMatrix() {
  for (int i = 0; i < numRows; i++) {
    delete rows[i];
  }
  delete[] rows;
}

IntMatrix & IntMatrix::operator=(const IntMatrix & rhs) {
  if (this != &rhs) {
    IntArray ** temp = new IntArray *[rhs.numRows];
    for (int i = 0; i < rhs.numRows; i++) {
      temp[i] = new IntArray(rhs.numColumns);
      for (int j = 0; j < rhs.numColumns; j++) {
        (*temp[i])[j] = rhs[i][j];
      }
    }

    for (int i = 0; i < numRows; i++) {
      delete rows[i];
    }
    delete[] rows;
    numRows = rhs.numRows;
    numColumns = rhs.numColumns;
    rows = temp;
  }
  return *this;
}
int IntMatrix::getRows() const {
  return numRows;
}
int IntMatrix::getColumns() const {
  return numColumns;
}
const IntArray & IntMatrix::operator[](int index) const {
  assert(index < numRows);
  return *(rows[index]);
}
IntArray & IntMatrix::operator[](int index) {
  assert(index < numRows);
  return *(rows[index]);
}

bool IntMatrix::operator==(const IntMatrix & rhs) const {
  if (rhs.numRows != numRows) {
    return false;
  }
  if (rhs.numColumns != numColumns) {
    return false;
  }
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numColumns; j++) {
      if (rhs[i][j] != (*this)[i][j]) {
        return false;
      }
    }
  }
  return true;
}

IntMatrix IntMatrix::operator+(const IntMatrix & rhs) const {
  assert(rhs.numRows == numRows);
  assert(rhs.numColumns == numColumns);

  IntMatrix ans(*this);

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numColumns; j++) {
      ans[i][j] += rhs[i][j];
    }
  }
  return ans;
}

std::ostream & operator<<(std::ostream & s, const IntMatrix & rhs) {
  if (rhs.getRows() == 0) {
    std::cout << "[]";
  }
  else {
    std::cout << "[ " << rhs[0];
    for (int i = 1; i < rhs.getRows(); i++) {
      std::cout << std::endl << rhs[i];
    }
    std::cout << " ]";
  }
  return s;
}
