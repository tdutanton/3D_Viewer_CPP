#ifndef SCENE_DRAWER_BASE_H
#define SCENE_DRAWER_BASE_H

namespace s21 {

class Scene;
class Figure;

/**
 * @class SceneDrawerBase
 * @brief Abstract base class for scene drawers.
 *
 * Concrete subclass of this class must implement the DrawFigure method to
 * handle the actual rendering of a figure.
 */
class SceneDrawerBase {
 public:
  // Constructor
  SceneDrawerBase() = default;
  virtual ~SceneDrawerBase(){};
  virtual void DrawFigure(Figure* figure) = 0;
};
}  // namespace s21

#endif
