#include "edge.h"

namespace s21 {

/**
 * @brief Equality comparison operator for Edge objects.
 *
 * This operator checks if two Edge objects are equal. Two edges are considered
 * equal if their begin and end points are the same, regardless of the order
 * (i.e., an edge from A to B is considered equal to an edge from B to A).
 *
 * @param other The Edge object to compare with.
 * @return true if the edges are equal, false otherwise.
 */
bool Edge::operator==(const Edge& other) const {
  return (begin_ == other.get_begin() && end_ == other.get_end()) ||
         (begin_ == other.get_end() && end_ == other.get_begin());
}

/**
 * @brief Assignment operator for Edge objects.
 *
 * This operator assigns the values of another Edge object to this object.
 * It performs a self-assignment check to ensure that assigning an object to
 * itself does not cause any issues.
 *
 * @param other The Edge object to assign from.
 * @return A reference to this Edge object after assignment.
 */
Edge& Edge::operator=(const Edge& other) {
  if (this != &other) {
    begin_ = other.begin_;
    end_ = other.end_;
  }
  return *this;
}

}  // namespace s21
