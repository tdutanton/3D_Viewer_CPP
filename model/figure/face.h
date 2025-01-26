#ifndef FACE_H_
#define FACE_H_

#include <vector>

#include "vertex.h"

namespace s21 {
/**
 * @class Face
 * @brief Represents a face composed of a vector of unsigned integers.
 *
 * The `Face` class encapsulates a face, which is represented as a vector of
 * unsigned integers.
 *
 * @note In test mode (`#ifdef TEST`), the member variables are public for
 * testing purposes.
 */
class Face {
#ifdef TEST
 public:
#else
 private:
#endif
  /// @brief Vector representing the face.
  std::vector<unsigned> face_;

 public:
  /// @brief Default constructor for the Face class. Initializes an empty face.
  Face() : face_(){};

  /**
   * @brief Parameterized constructor for the Face class.
   * Initializes the face with the provided vector of unsigned integers.
   * @param face A vector of unsigned integers representing the face.
   */
  Face(std::vector<unsigned> face) : face_(face) {}

  /// @brief Destructor for the Face class. Default destructor
  ~Face() = default;

  /**
   * @brief Getter for the face vector.
   * @return A copy of the vector representing the face.
   */
  std::vector<unsigned> get_face() const { return face_; }

  /**
   * @brief Equality comparison operator for Face objects.
   * Compares two Face objects for equality based on their internal face
   * vectors.
   * @param other The Face object to compare with.
   * @return true if the face vectors are equal, false otherwise.
   */
  bool operator==(const Face& other) const { return face_ == other.face_; }
};

}  // namespace s21

#endif