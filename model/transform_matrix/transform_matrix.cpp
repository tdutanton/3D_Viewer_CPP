#include "transform_matrix.h"

namespace s21 {

/**
 * @brief Default constructor for TransformMatrix.
 */
TransformMatrix::TransformMatrix() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrix_[i][j] = (i == j) ? 1 : 0;
    }
  }
}

/**
 * @brief Resets the matrix to a zero matrix. All elements of the matrix are set
 * to 0.
 */
void TransformMatrix::reset() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrix_[i][j] = 0;
    }
  }
}

/**
 * @brief Overloads the multiplication operator for matrix multiplication.
 *
 * Multiplies this matrix with another matrix and returns the result.
 *
 * @param other_matrix The matrix to multiply with.
 * @return A new TransformMatrix representing the result of the multiplication.
 */
TransformMatrix TransformMatrix::operator*(
    const TransformMatrix& other_matrix) const {
  TransformMatrix result;
  result.reset();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int k = 0;
      while (k < 4) {
        result.matrix_[i][j] += matrix_[i][k] * other_matrix.matrix_[k][j];
        k++;
      }
    }
  }
  return result;
}

/**
 * @brief Sets the value of a matrix element at the specified row and column.
 *
 * @param row The row index (starts from 0).
 * @param col The column index (starts from 0).
 * @param v The value to set.
 * @throws std::out_of_range If the row or column index is out of bounds.
 */
void TransformMatrix::set_elem(int row, int col, float v) {
  if (row >= 0 && row < 4 && col >= 0 && col < 4) {
    matrix_[row][col] = v;
  } else {
    throw std::out_of_range("Wrong index");
  }
}

/**
 * @brief Overloads the multiplication operator for transforming a point.
 *
 * Applies the transformation matrix to a point and returns the transformed
 * point.
 *
 * @param point The point to transform.
 * @return A new Point representing the transformed point.
 */
Point TransformMatrix::operator*(const Point& point) const {
  Point result(0, 0, 0);
  result.x_ = get_elem(0, 0) * point.x_ + get_elem(0, 1) * point.y_ +
              get_elem(0, 2) * point.z_ + get_elem(0, 3) * point.w_;

  result.y_ = get_elem(1, 0) * point.x_ + get_elem(1, 1) * point.y_ +
              get_elem(1, 2) * point.z_ + get_elem(1, 3) * point.w_;

  result.z_ = get_elem(2, 0) * point.x_ + get_elem(2, 1) * point.y_ +
              get_elem(2, 2) * point.z_ + get_elem(2, 3) * point.w_;

  result.w_ = get_elem(3, 0) * point.x_ + get_elem(3, 1) * point.y_ +
              get_elem(3, 2) * point.z_ + get_elem(3, 3) * point.w_;
  return result;
}

/**
 * @brief Transforms a point using the transformation matrix.
 *
 * @param point The point to transform.
 * @return A new Point representing the transformed point.
 */
Point TransformMatrix::transform_point(Point& point) { return *this * point; }

/**
 * @brief Creates a rotation matrix for rotation around the X-axis.
 *
 * @param angle The rotation angle in radians.
 * @return A TransformMatrix representing the X-axis rotation.
 */
TransformMatrix TransformMatrixBuilder::create_only_x_rotation_matrix(
    float angle) {
  TransformMatrix result;
  result.set_elem(1, 1, cos(angle));
  result.set_elem(1, 2, -sin(angle));
  result.set_elem(2, 1, sin(angle));
  result.set_elem(2, 2, cos(angle));
  return result;
}

/**
 * @brief Creates a rotation matrix for rotation around the Y-axis.
 *
 * @param angle The rotation angle in radians.
 * @return A TransformMatrix representing the Y-axis rotation.
 */
TransformMatrix TransformMatrixBuilder::create_only_y_rotation_matrix(
    float angle) {
  TransformMatrix result;
  result.set_elem(0, 0, cos(-angle));
  result.set_elem(0, 2, -sin(-angle));
  result.set_elem(2, 0, sin(-angle));
  result.set_elem(2, 2, cos(-angle));
  return result;
}

/**
 * @brief Creates a rotation matrix for rotation around the Z-axis.
 *
 * @param angle The rotation angle in radians.
 * @return A TransformMatrix representing the Z-axis rotation.
 */
TransformMatrix TransformMatrixBuilder::create_only_z_rotation_matrix(
    float angle) {
  TransformMatrix result;
  result.set_elem(0, 0, cos(angle));
  result.set_elem(0, 1, -sin(angle));
  result.set_elem(1, 0, sin(angle));
  result.set_elem(1, 1, cos(angle));
  return result;
}

/**
 * @brief Creates a combined rotation matrix for rotations around all three
 * axes.
 *
 * Look at the order - it must be: x -> y -> z!
 *
 * @param x_degree The rotation angle around the X-axis in degrees.
 * @param y_degree The rotation angle around the Y-axis in degrees.
 * @param z_degree The rotation angle around the Z-axis in degrees.
 * @return A TransformMatrix representing the combined rotation.
 */
TransformMatrix TransformMatrixBuilder::create_rotation_matrix(float x_degree,
                                                               float y_degree,
                                                               float z_degree) {
  return create_only_z_rotation_matrix(deg_to_rad(z_degree)) *
         create_only_y_rotation_matrix(deg_to_rad(y_degree)) *
         create_only_x_rotation_matrix(deg_to_rad(x_degree));
}

/**
 * @brief Creates a moving matrix for moving points in 3D space.
 *
 * @param x The distance along the X-axis.
 * @param y The distance along the Y-axis.
 * @param z The distance along the Z-axis.
 * @return A TransformMatrix representing the moving.
 */
TransformMatrix TransformMatrixBuilder::create_move_matrix(float x, float y,
                                                           float z) {
  TransformMatrix result;
  result.set_elem(0, 3, x);
  result.set_elem(1, 3, y);
  result.set_elem(2, 3, z);
  return result;
}

/**
 * @brief Creates a scaling matrix for scaling points in 3D space.
 *
 * @param x The scaling factor along the X-axis.
 * @param y The scaling factor along the Y-axis.
 * @param z The scaling factor along the Z-axis.
 * @return A TransformMatrix representing the scaling.
 */
TransformMatrix TransformMatrixBuilder::create_scale_matrix(float x, float y,
                                                            float z) {
  TransformMatrix result;
  result.set_elem(0, 0, x);
  result.set_elem(1, 1, y);
  result.set_elem(2, 2, z);
  return result;
}

}  // namespace s21