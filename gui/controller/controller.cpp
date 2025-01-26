#include "controller.h"

namespace s21 {

/**
 * @brief Constructs a new Controller instance with the given Facade.
 * @param facade Pointer to Facade object for using by Controller.
 */
Controller::Controller(Facade *facade) { facade_ = facade; }

/**
 * @brief Scales the model by the given factor in all three dimensions.
 * @param x The scaling factor to apply to the model.
 */
void Controller::scale_model(float x) { facade_->scale_figure(x, x, x); }

/**
 * @brief Moves the model by the given offsets in the x, y, and z dimensions.
 * @param x The offset to apply to the model's x-coordinate.
 * @param y The offset to apply to the model's y-coordinate.
 * @param z The offset to apply to the model's z-coordinate.
 */
void Controller::move_model(float x, float y, float z) {
  facade_->move_figure(x, y, z);
}

/**
 * @brief Rotates the model by the given angles around the x, y, and z axes.
 * @param x The angle (in degrees) to rotate the model around the x-axis.
 * @param y The angle (in degrees) to rotate the model around the y-axis.
 * @param z The angle (in degrees) to rotate the model around the z-axis.
 */
void Controller::rotate_model(float x, float y, float z) {
  facade_->rotate_figure(x, y, z);
}

/**
 * @brief Loads a wireframe model from given file path and puts it in the
 * Facade.
 * @param file_path The file path for loading the 3d model.
 */
void Controller::read_model(std::string file_path) {
  facade_->load_figure(file_path);
}

/**
 * @brief Resets the position of the 3D model to its start position.
 */
void Controller::reset_model_pos() { facade_->reset_figure_pos(); }

/**
 * @brief Returns a vector containing the number of vertices, total edges, and
 * unique edges in the 3D model.
 * @return A vector of integers representing the vertex count, total edge count,
 * and unique edge count of the 3D model.
 */
std::vector<int> Controller::get_model_data() {
  return {facade_->get_vertices_count(), facade_->get_all_edges_count(),
          facade_->get_unique_edges_count()};
}

/**
 * @brief Returns pointer to Figure object from Facade for drawing by
 * QtSceneDrawer.
 * @return A pointer to Figure object.
 */
Figure *Controller::get_figure() { return facade_->get_figure(); }

}  // namespace s21
