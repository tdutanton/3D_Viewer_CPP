#include "figure.h"

namespace s21 {

/**
 * @brief Transforms the figure's vertices using the provided transformation
 * matrix. This method applies the transformation matrix to each vertex in the
 * figure.
 * @param matrix The transformation matrix to apply.
 */
void Figure::transform(const TransformMatrix& matrix) {
  for (auto& v : vertices_) {
    v.transform(matrix);
  }
}

/**
 * @brief Hash function for pairs of values.
 * This struct provides a hash function for pairs of values, used to store edges
 * in a hash set.
 */
struct PairHash {
  template <typename T1, typename T2>
  std::size_t operator()(const std::pair<T1, T2>& p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);
    return h1 ^ (h2 << 1);
  }
};

/**
 * @brief Generates the edges of the figure based on its vertices and faces.
 *
 * This method clears the existing edges and generates new edges by iterating
 * through the faces and connecting vertices. It ensures that each edge is
 * unique and updates the total count of edges.
 */
void Figure::make_edges() {
  edges_.clear();
  all_edges_count_ = 0;
  std::unordered_set<std::pair<size_t, size_t>, PairHash> unique_edges;
  for (const auto& face : faces_) {
    const auto& vertices = face.get_face();
    const size_t face_size = vertices.size();
    for (size_t i = 0; i < face_size; i++) {
      const size_t v1 = vertices[i];
      const size_t v2 = vertices[(i + 1) % face_size];
      if (v1 < vertices_.size() && v2 < vertices_.size()) {
        all_edges_count_++;
        auto edge = std::minmax(v1, v2);
        if (unique_edges.insert(edge).second) {
          edges_.emplace_back(vertices_[edge.first], vertices_[edge.second]);
        }
      }
    }
  }
}

/// @brief Saves the current vertices as the default (original) coordinates.
void Figure::save_default_coords() { default_vertices_ = vertices_; }

/// @brief Restores the vertices to their default (original) coordinates.
void Figure::get_default_coords() { vertices_ = default_vertices_; }

}  // namespace s21