#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../../model/facade/facade.h"

namespace s21 {
/**
 * @class Controller
 * @brief Provides an interface for interacting with Model in MVC pattern.
 *
 * The Controller class is responsible for managing the interaction between the
 * GUI and the underlying 3D model. It provides methods for scaling, moving,
 * rotating, and reading the model, as well as resetting its position and
 * retrieving its data.
 */
class Controller {
 public:
  // Constructor
  Controller(Facade* facade);
  // Destructor
  ~Controller() = default;

  void scale_model(float x);
  void move_model(float x, float y, float z);
  void rotate_model(float x, float y, float z);
  void read_model(std::string file_path);
  void reset_model_pos();
  std::vector<int> get_model_data();
  Figure* get_figure();

 private:
  // Pointer to Model object
  Facade* facade_;
};
}  // namespace s21

#endif  // CONTROLLER_H
