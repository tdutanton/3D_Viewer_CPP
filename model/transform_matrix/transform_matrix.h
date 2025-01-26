#ifndef TRANSFORM_MATRIX_H_
#define TRANSFORM_MATRIX_H_

#include <cmath>
#include <iostream>
#include <stdexcept>

#include "../figure/figure.h"

/**
 * @brief Service function for getting radians from degrees. Use in
 * transformation functions for matrix.
 *
 * @param angle Angle (degrees) which need to be converted to radians.
 * @return Radians - float type.
 */
inline float deg_to_rad(float angle) { return M_PI * angle / 180.0f; }

namespace s21 {
typedef struct Point Point;

/**
 * @class TransformMatrix
 * @brief A class representing a 4x4 transformation matrix.
 *
 * This class provides functionality to manage and manipulate a 4x4
 * transformation matrix.
 *
 * @note In test mode (`#ifdef TEST`), the member variables are public for
 * testing purposes.
 */
class TransformMatrix {
#ifdef TEST
 public:
#else
 private:
#endif
  /// @brief 4x4 matrix storing transformation data.
  float matrix_[4][4];

 public:
  TransformMatrix();

  /// @brief Default destructor.
  ~TransformMatrix() = default;
  void reset();

  /**
   * @brief Overloads the multiplication operator for matrix multiplication.
   *
   * @param other_matrix The matrix to multiply with.
   * @return A new TransformMatrix representing the result of the
   * multiplication.
   */
  TransformMatrix operator*(const TransformMatrix& other_matrix) const;
  Point operator*(const Point& point) const;
  Point transform_point(Point& point);

  /**
   * @brief Gets the value of a matrix element at the specified row and column.
   *
   * @param row The row index (starts from 0).
   * @param col The column index (starts from 0).
   * @return The value of the matrix element at the specified position.
   */
  float get_elem(int row, int col) const { return matrix_[row][col]; }
  void set_elem(int row, int col, float v);
};

/**
 * @class TransformMatrixBuilder
 * @brief A utility class for creating transformation matrices.
 *
 * This class provides static methods to generate specific types of
 * transformation matrices. This class works like "builder" pattern.
 */
class TransformMatrixBuilder {
 public:
  static TransformMatrix create_only_x_rotation_matrix(float angle);
  static TransformMatrix create_only_y_rotation_matrix(float angle);
  static TransformMatrix create_only_z_rotation_matrix(float angle);

  static TransformMatrix create_rotation_matrix(float x_degree, float y_degree,
                                                float z_degree);
  static TransformMatrix create_move_matrix(float x, float y, float z);
  static TransformMatrix create_scale_matrix(float x, float y, float z);
};

}  // namespace s21

#endif