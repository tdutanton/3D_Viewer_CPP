#ifndef VERTEX_H_
#define VERTEX_H_

#include "../transform_matrix/transform_matrix.h"
#include "scene_object.h"

namespace s21 {

class TransformMatrix;

/**
 * @struct Point
 * @brief Represents a point in 3D space with homogeneous coordinates.
 *
 * The `Point` struct defines a point with coordinates (x, y, z, w), where w is
 * the homogeneous coordinate (default is 1.0). It provides methods for
 * transformation, comparison, and printing, as well as getters and setters for
 * the coordinates.
 */
typedef struct Point {
  /// The x-coordinate of the point.
  float x_;
  /// The y-coordinate of the point.
  float y_;
  /// The z-coordinate of the point.
  float z_;
  /// The homogeneous coordinate (default is 1.0).
  float w_;

  /**
   * @brief Default constructor for the Point struct.
   * Initializes the point to (0, 0, 0, 1).
   */
  Point() : x_(0), y_(0), z_(0), w_(1) {}

  /**
   * @brief Parameterized constructor for the Point struct.
   * Initializes the point with the provided coordinates.
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @param z The z-coordinate.
   * @param w The homogeneous coordinate (default is 1.0).
   */
  Point(float x, float y, float z, float w = 1.0)
      : x_(x), y_(y), z_(z), w_(w) {}

  Point operator*(const TransformMatrix& matrix) const;
  bool operator==(const Point& other) const;
  void print_point() const;

  /**
   * @brief Gets the x-coordinate of the point.
   * @return The x-coordinate.
   */
  float get_x() const { return x_; }

  /**
   * @brief Gets the y-coordinate of the point.
   * @return The y-coordinate.
   */
  float get_y() const { return y_; }

  /**
   * @brief Gets the z-coordinate of the point.
   * @return The z-coordinate.
   */
  float get_z() const { return z_; }

  /**
   * @brief Gets the homogeneous coordinate (w) of the point.
   * @return The homogeneous coordinate.
   */
  float get_w() const { return w_; }

  /**
   * @brief Sets the x-coordinate of the point.
   * @param x The new x-coordinate.
   */
  void set_x(float x) { x_ = x; }

  /**
   * @brief Sets the y-coordinate of the point.
   * @param y The new y-coordinate.
   */
  void set_y(float y) { y_ = y; }

  /**
   * @brief Sets the z-coordinate of the point.
   * @param z The new z-coordinate.
   */
  void set_z(float z) { z_ = z; }
} Point;

/**
 * @class Vertex
 * @brief Represents a vertex in 3D space, derived from SceneObject.
 *
 * The `Vertex` class encapsulates a point in 3D space and provides
 * functionality for transforming the vertex and comparing it with other
 * vertices.
 * @note In test mode (`#ifdef TEST`), the member variables are public for
 * testing purposes.
 */
class Vertex : public SceneObject {
#ifdef TEST
 public:
#else
 private:
#endif
  /// The position of the vertex in 3D space.
  Point positions_;

 public:
  /**
   * @brief Default constructor for the Vertex class.
   * Initializes the vertex with a default point (0, 0, 0, 1).
   */
  Vertex() : positions_() {}

  /**
   * @brief Parameterized constructor for the Vertex class.
   * Initializes the vertex with the provided point.
   * @param p The point representing the vertex's position.
   */
  explicit Vertex(Point p) : positions_(p) {}

  /// @brief Destructor for the Vertex class.
  ~Vertex() = default;

  /**
   * @brief Gets the position of the vertex.
   * @return The point representing the vertex's position.
   */
  Point get_position() const { return positions_; }

  /**
   * @brief Sets the position of the vertex.
   * @param point The new position of the vertex.
   */
  void set_position(Point point) { positions_ = point; }

  void transform(const TransformMatrix& matrix) override;
  bool operator==(const Vertex& other) const;
};

}  // namespace s21

#endif