#include "facade.h"

namespace s21 {

/**
 * @brief Loads a wireframe model from the specified file path.
 *
 * @param path The file path of figure to load.
 * @return A FacadeOperationResult object containing the result of the
 * operation.
 */
FacadeOperationResult Facade::load_figure(std::string path) {
  FacadeOperationResult res;
  try {
    if (!fileReader_ || !figure_) {
      res.SetSuccess(false);
      res.SetErrorMessage("Internal error: fileReader or figure is null.");
      return res;
    }
    NormalizationParameters* params = NormalizationParameters::instance();
    params->reset_normalization_parameters();
    res = reset_figure();
    *figure_ = fileReader_->read_figure(path, params);
    if (fileReader_->read_successfull()) {
      figure_->make_edges();
      figure_->save_default_coords();
      res.SetSuccess(true);
    }
  } catch (const std::exception& e) {
    res.SetSuccess(false);
    res.SetErrorMessage("File reading error");
  }
  return res;
}

/**
 * @brief Deletes old figure object and creates new one.
 * @return A FacadeOperationResult object containing the result of the
 * operation.
 */
FacadeOperationResult Facade::reset_figure() {
  FacadeOperationResult res;
  delete figure_;
  figure_ = new Figure();
  return FacadeOperationResult();
}

/**
 * @brief Moves the figure by the specified x, y, and z coordinates.
 *
 * @param x The x-coordinate to move the figure by.
 * @param y The y-coordinate to move the figure by.
 * @param z The z-coordinate to move the figure by.
 * @return A FacadeOperationResult object containing the result of the
 * operation.
 */
FacadeOperationResult Facade::move_figure(float x, float y, float z) {
  FacadeOperationResult res;
  TransformMatrix matrix = TransformMatrixBuilder::create_move_matrix(x, y, z);
  figure_->transform(matrix);
  return res;
}

/**
 * @brief Rotates the figure by the specified x, y, and z angles.
 *
 * @param x The angle to rotate the figure around the x-axis.
 * @param y The angle to rotate the figure around the y-axis.
 * @param z The angle to rotate the figure around the z-axis.
 * @return A FacadeOperationResult object containing the result of the
 * operation.
 */
FacadeOperationResult Facade::rotate_figure(float x, float y, float z) {
  FacadeOperationResult res;
  TransformMatrix matrix =
      TransformMatrixBuilder::create_rotation_matrix(x, y, z);
  figure_->transform(matrix);
  return res;
}

/**
 * @brief Scales the figure by the specified x, y, and z factors.
 *
 * @param x The scaling factor for the x-axis.
 * @param y The scaling factor for the y-axis.
 * @param z The scaling factor for the z-axis.
 * @return A FacadeOperationResult object containing the result of the
 * operation.
 */
FacadeOperationResult Facade::scale_figure(float x, float y, float z) {
  FacadeOperationResult res;
  TransformMatrix matrix = TransformMatrixBuilder::create_scale_matrix(x, y, z);
  figure_->transform(matrix);
  return res;
}

}  // namespace s21
