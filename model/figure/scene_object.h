#ifndef SCENE_OBJECT_H_
#define SCENE_OBJECT_H_

#include "../transform_matrix/transform_matrix.h"

namespace s21 {

class TransformMatrix;

/**
 * @class SceneObject
 * @brief Abstract base class representing a scene object.
 *
 * The `SceneObject` class serves as a base class for all objects in a 3D scene.
 * It defines an interface for transforming objects using a transformation
 * matrix. Classes derived from `SceneObject` must implement the `transform`
 * method.
 */
class SceneObject {
 public:
  /**
   * @brief Virtual destructor for the SceneObject class.
   * Ensures proper cleanup of derived class objects when deleted through a base
   * class pointer.
   */
  virtual ~SceneObject() = default;

  /**
   * @brief Pure virtual method to transform the scene object.
   * This method must be implemented by derived classes to apply a
   * transformation matrix to the object.
   * @param matrix The transformation matrix to apply.
   */
  virtual void transform(const TransformMatrix& matrix) = 0;
};

}  // namespace s21

#endif
