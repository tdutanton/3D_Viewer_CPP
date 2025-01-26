#ifndef FACADE_H_
#define FACADE_H_

#include "../figure/figure.h"
#include "../file_reader/file_reader.h"
#include "../transform_matrix/transform_matrix.h"
#include "normalization_parameters.h"
#include "scene_drawer_base.h"

namespace s21 {

/**
 * @class FacadeOperationResult
 * @brief Represents the result of an operation performed by the Facade class.
 *
 * This class contains the success or failure of an operation and optional error
 * message.
 */
class FacadeOperationResult {
 private:
  std::string errorMessage_;
  bool isSuccess_;

 public:
  // Default constructor
  FacadeOperationResult() {
    errorMessage_ = "";
    isSuccess_ = true;
  };
  // Destructor
  ~FacadeOperationResult() = default;
  // Getters and setters
  std::string GetErrorMessage() const { return errorMessage_; };
  bool IsSuccess() const { return isSuccess_; };
  void SetErrorMessage(std::string message) { errorMessage_ = message; };
  void SetSuccess(bool status) { isSuccess_ = status; };
};

/**
 * @class Facade
 * @brief Represents interface for interacting with Controller class.
 *
 * The Facade class is central point of access to model part of MVC pattern.
 * Contains FileReader, Figure and SceneDrawerBase. It
 * provides API for operations like loading, moving,
 * rotating, and scaling a 3D figure.
 */
class Facade {
#ifdef TEST
 public:
#else
 private:
#endif
  // Pointer to FileReader object, which is responsible for reading .obj files.
  FileReader* fileReader_;
  // Pointer to scene drawer.
  SceneDrawerBase* sceneDrawer_;
  // Pointer to Figure object. Keeps all information about wireframe model.
  Figure* figure_;

 public:
  // Default Facade constructor
  Facade() {
    figure_ = new Figure();
    fileReader_ = new FileReader();
  };
  // Facade Constructor with scene drawer. In this implementation is not used.
  Facade(SceneDrawerBase* sceneDrawer) : sceneDrawer_(sceneDrawer) {
    figure_ = new Figure();
    fileReader_ = new FileReader();
  };
  // Facade Destructor
  ~Facade() = default;

  FacadeOperationResult load_figure(std::string path);
  FacadeOperationResult reset_figure();
  FacadeOperationResult move_figure(float x, float y, float z);
  FacadeOperationResult rotate_figure(float x, float y, float z);
  FacadeOperationResult scale_figure(float x, float y, float z);

  // returns number of vertices
  int get_vertices_count() { return figure_->get_vertices().size(); };
  // returns number of edges
  int get_unique_edges_count() { return figure_->get_edges().size(); };
  // returns number of unique edges
  int get_all_edges_count() { return figure_->get_all_edges_count(); };
  // returns pointer to figure object for drawing
  Figure* get_figure() { return figure_; };
  // set default position of figure
  void reset_figure_pos() { figure_->get_default_coords(); };
};

}  // namespace s21

#endif