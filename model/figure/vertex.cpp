#include "vertex.h"

namespace s21 {

/**
 * @brief Transforms the vertex by multiplying its position with the
 * transformation matrix. This method applies the transformation matrix to the
 * vertex's position, updating its coordinates.
 * @param matrix The transformation matrix to apply.
 */
void Vertex::transform(const TransformMatrix& matrix) {
  positions_ = positions_ * matrix;
}

/**
 * @brief Compares two vertices for equality.
 * Two vertices are considered equal if their positions are the same.
 * @param other The vertex to compare with.
 * @return true if the vertices are equal, false otherwise.
 */
bool Vertex::operator==(const Vertex& other) const {
  return positions_ == other.positions_;
}

/// @brief Prints the point's coordinates to the standard output.
void Point::print_point() const {
  std::cout << "x = " << x_ << " y = " << y_ << " z = " << z_ << std::endl;
}

/**
 * @brief Multiplies the point by a transformation matrix.
 * This method applies the transformation matrix to the point, resulting in a
 * new point with transformed coordinates.
 * @param matrix The transformation matrix to apply.
 * @return A new Point representing the transformed point.
 */
Point Point::operator*(const TransformMatrix& matrix) const {
  Point result(0, 0, 0);
  result.x_ = matrix.get_elem(0, 0) * x_ + matrix.get_elem(0, 1) * y_ +
              matrix.get_elem(0, 2) * z_ + matrix.get_elem(0, 3) * w_;

  result.y_ = matrix.get_elem(1, 0) * x_ + matrix.get_elem(1, 1) * y_ +
              matrix.get_elem(1, 2) * z_ + matrix.get_elem(1, 3) * w_;

  result.z_ = matrix.get_elem(2, 0) * x_ + matrix.get_elem(2, 1) * y_ +
              matrix.get_elem(2, 2) * z_ + matrix.get_elem(2, 3) * w_;

  result.w_ = matrix.get_elem(3, 0) * x_ + matrix.get_elem(3, 1) * y_ +
              matrix.get_elem(3, 2) * z_ + matrix.get_elem(3, 3) * w_;
  return result;
}

/**
 * @brief Compares two points for equality.
 * Two points are considered equal if all their coordinates (x, y, z, w) are the
 * same.
 * @param other The point to compare with.
 * @return true if the points are equal, false otherwise.
 */
bool Point::operator==(const Point& other) const {
  return (x_ == other.x_) && (y_ == other.y_) && (z_ == other.z_) &&
         (w_ == other.w_);
}

}  // namespace s21