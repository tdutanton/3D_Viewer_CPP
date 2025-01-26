#ifndef FIGURE_H_
#define FIGURE_H_

#include <algorithm>
#include <unordered_set>

#include "edge.h"
#include "face.h"
#include "scene_object.h"
#include "vertex.h"

namespace s21 {

struct PairHash;

class Vertex;
class Edge;

/**
 * @class Figure
 * @brief Represents a figure in a 3D scene, derived from SceneObject.
 *
 * The `Figure` class encapsulates a 3D figure, including its vertices, faces,
 * edges, and name. It provides functionality for transforming the figure,
 * printing its properties, and managing its geometry. The class is a friend of
 * `FileReader` and `Facade` to allow controlled access to its private members.
 *
 * @note In test mode (`#ifdef TEST`), the member variables are public for
 * testing purposes.
 */
class Figure : public SceneObject {
  friend class FileReader;
  friend class Facade;

#ifdef TEST
 public:
#else
 private:
#endif
  /// Vector of vertices defining the figure.
  std::vector<Vertex> vertices_;
  /// Vector of vertices in their default (original) positions.
  std::vector<Vertex> default_vertices_;
  /// Name of the figure.
  std::string name_;
  /// Vector of faces defining the figure.
  std::vector<Face> faces_;
  /// Vector of edges defining the figure.
  std::vector<Edge> edges_;
  /// Total count of edges in the figure.
  unsigned long long all_edges_count_;

 public:
  /// @brief Default constructor for the Figure class.
  Figure() = default;
  /// @brief Default destructor for the Figure class.
  ~Figure() = default;

  /**
   * @brief Sets the name of the figure.
   * @param name The name to assign to the figure.
   */
  void set_name(std::string name) { name_ = name; }

  /**
   * @brief Gets the vertices of the figure.
   * @return A vector of vertices defining the figure.
   */
  std::vector<Vertex> get_vertices() const { return vertices_; }

  /**
   * @brief Gets the name of the figure.
   * @return The name of the figure.
   */
  std::string get_name() const { return name_; }

  /**
   * @brief Gets the edges of the figure.
   * @return A vector of edges defining the figure.
   */
  std::vector<Edge> get_edges() const { return edges_; }

  /**
   * @brief Gets the faces of the figure.
   * @return A vector of faces defining the figure.
   */
  std::vector<Face> get_faces() const { return faces_; }

  /**
   * @brief Gets the total count of edges in the figure.
   * @return The total count of edges.
   */
  unsigned long long get_all_edges_count() const { return all_edges_count_; }
  void transform(const TransformMatrix& matrix) override;

  void make_edges();
  void save_default_coords();
  void get_default_coords();
};

}  // namespace s21

#endif