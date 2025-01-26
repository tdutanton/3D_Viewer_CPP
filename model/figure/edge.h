#ifndef EDGE_H_
#define EDGE_H_

#include "vertex.h"

namespace s21 {

class Vertex;

/**
 * @class Edge
 * @brief Represents an edge in a figure, connecting two vertices.
 *
 * This class models a directed edge in a graph structure. Each edge has a
 * starting vertex (`begin_`) and an ending vertex (`end_`).
 *
 * @note In test mode (`#ifdef TEST`), the member variables are public for
 * testing purposes.
 */
class Edge {
#ifdef TEST
 public:
#else
 private:
#endif
  /// @brief The starting vertex of the edge.
  Vertex& begin_;

  /// @brief The ending vertex of the edge.
  Vertex& end_;

 public:
  /**
   * @brief Constructs an Edge object connecting two vertices.
   * @param begin The starting vertex of the edge.
   * @param end The ending vertex of the edge.
   */
  Edge(Vertex& begin, Vertex& end) : begin_(begin), end_(end) {}

  /// @brief Default destructor.
  ~Edge() = default;

  /**
   * @brief Gets the starting vertex of the edge.
   * @return A reference to the starting vertex.
   */
  Vertex& get_begin() const { return begin_; }

  /**
   * @brief Gets the ending vertex of the edge.
   * @return A reference to the ending vertex.
   */
  Vertex& get_end() const { return end_; }

  bool operator==(const Edge& other) const;
  Edge& operator=(const Edge& other);
};

}  // namespace s21

#endif