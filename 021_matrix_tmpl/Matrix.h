#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <assert.h>

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;
//YOUR CODE GOES HERE!
template<typename T>
class Matrix {
 private:
  int numRows;
  int numColumns;
  vector<vector<T> > rows;

 public:
  Matrix() : numRows(0), numColumns(0), rows(vector<vector<T> >(0)) {}
  Matrix(int r, int c) :
      numRows(r),
      numColumns(c),
      rows(vector<vector<T> >(r, vector<T>(c))) {}
  Matrix(const Matrix & rhs) :
      numRows(rhs.numRows),
      numColumns(rhs.numColumns),
      rows(rhs.rows) {}

  ~Matrix() {}
  Matrix & operator=(const Matrix<T> & rhs) {
    if (this != &rhs) {
      numRows = rhs.numRows;
      numColumns = rhs.numColumns;
      rows = rhs.rows;
    }
    return *this;
  }
  int getRows() const { return numRows; }
  int getColumns() const { return numColumns; }
  const vector<T> & operator[](int index) const {
    assert(index < numRows);
    assert(index >= 0);
    return rows[index];
  }
  vector<T> & operator[](int index) {
    assert(index < numRows);
    assert(index >= 0);
    return rows[index];
  }

  bool operator==(const Matrix & rhs) const {
    if (rhs.numRows != numRows) {
      return false;
    }
    if (rhs.numColumns != numColumns) {
      return false;
    }
    for (int i = 0; i < numRows; i++) {
      if (rhs[i] != (*this)[i]) {
        return false;
      }
    }

    return true;
  }

  Matrix operator+(const Matrix & rhs) const {
    assert(rhs.numRows == numRows);
    assert(rhs.numColumns == numColumns);

    Matrix ans(*this);

    for (int i = 0; i < numRows; i++) {
      for (int j = 0; j < numColumns; j++) {
        ans[i][j] += rhs[i][j];
      }
    }
    return ans;
  }

  friend std::ostream & operator<<(std::ostream & s, const Matrix<T> & rhs) {
    if (rhs.getRows() == 0) {
      cout << "[  ]";
    }
    else {
      cout << "[ ";
      for (int i = 0; i < rhs.getRows(); i++) {
        if (rhs.getColumns() == 0) {
          cout << "{}";
        }
        else {
          cout << "{";
          for (int j = 0; j < rhs.getColumns(); j++) {
            cout << rhs[i][j] << ((j != rhs.getColumns() - 1) ? ", " : "}");
          }
          cout << ((i != rhs.getRows() - 1) ? ",\n" : " ]");
        }
      }
    }
    return s;
  }
};

#endif
