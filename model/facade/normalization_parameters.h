#ifndef NORMALIZATION_PARAMETERS_H_
#define NORMALIZATION_PARAMETERS_H_

namespace s21 {

/**
 * @class NormalizationParameters
 * @brief Manages the normalization parameters for Figure objects.
 *
 * This class provides a singleton instance that holds the minimum and maximum
 * values for the x, y, and z axes, as well as the step sizes for each axis.
 */
class NormalizationParameters {
 public:
  // Destructor
  ~NormalizationParameters() = default;
  /**
   * @brief Implementation of the singleton instance of the
   * NormalizationParameters class.
   * @return A pointer to the only one NormalizationParameters object.
   */
  static NormalizationParameters* instance() {
    static NormalizationParameters instance;
    return &instance;
  };
  float x_min, x_max, dx_step;
  float y_min, y_max, dy_step;
  float z_min, z_max, dz_step;
  float max_range;

  void reset_normalization_parameters() {
    x_min = 0;
    x_max = 0;
    dx_step = 0.01;
    y_min = 0;
    y_max = 0;
    dy_step = 0.01;
    z_min = 0;
    z_max = 0;
    dz_step = 0.01;
    max_range = 0;
  }

#ifdef TEST
 public:
#else
 protected:
#endif
  NormalizationParameters()
      : x_min(0),
        x_max(0),
        dx_step(0.01),
        y_min(0),
        y_max(0),
        dy_step(0.01),
        z_min(0),
        z_max(0),
        dz_step(0.01),
        max_range(0){};
  NormalizationParameters(NormalizationParameters const&) = delete;
  NormalizationParameters& operator=(NormalizationParameters const&) = delete;
};
}  // namespace s21

#endif