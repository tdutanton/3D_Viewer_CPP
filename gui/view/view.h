#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QPushButton>
#include <QWidget>
#include <QtWidgets>

#include "../controller/controller.h"
#include "qgifimage.h"
#include "qt_scene_drawer.h"
#include "settings_widget.h"
#include "widget_utils.h"

// 3d model start position values
#define START_SCALE 100.0f
#define START_POS 0.0f
#define START_ANGLE 0.0f

namespace s21 {
/**
 * @class View
 * @brief The View class is responsible for the GUI of
 * the 3D viewer application and handling user interactions, such as opening and
 * saving files, adjusting the 3D object's scale, position, and rotation,
 * switching between parallel and central projection modes. It also
 * includes a QtSceneDrawer object to render the 3D scene and a SettingsWidget
 * object to manage the application's settings.
 */
class View : public QWidget {
  Q_OBJECT
 public:
  /**
   * @enum ControlRanges
   * @brief Defines the minimum and maximum values for the scale, move, and
   * rotate controls used in the View class.
   */
  enum ControlRanges {
    SCALE_MIN = 1,
    SCALE_MAX = 500,
    MOVE_MIN = -200,
    MOVE_MAX = 200,
    ROTATE_MIN = -180,
    ROTATE_MAX = 180
  };

  /**
   * @struct PrevPositions
   * @brief Keeps track of the previous positions of the scale, move, and
   * rotate 3D object.
   */
  struct PrevPositions {
    float prev_scale = START_SCALE / 100.0;
    float prev_x_pos = START_POS;
    float prev_y_pos = START_POS;
    float prev_z_pos = START_POS;
    float prev_x_angle = START_ANGLE;
    float prev_y_angle = START_ANGLE;
    float prev_z_angle = START_ANGLE;
  };

  explicit View(QWidget *parent = nullptr, Controller *controller = nullptr);
  ~View();

 private:
  // Pointer to controller
  Controller *controller_;
  // OpenGL 3d model drawer
  QtSceneDrawer *drawer_;
  PrevPositions prev_pos_{};
  // Rendering parameters (projection, colors, edge and vertex type, size)
  QtSceneDrawer::RendParams *rend_params_;
  // Settings QDialog widget for changing rendering parameters
  SettingsWidget *view_settings_;
  // Timer for updating the 3D model
  QTimer *update_timer_;
  // Timer for tracking GIF duration
  QTimer *record_gif_timer_;
  // Timer for delay between GIF frames
  QTimer *gif_delay_timer_;
  // GIF image
  QGifImage *gif_;

  // GUI elements and controls
  // b - button, scb - scrollbar, spb - spinbox, lb - label

  QPushButton *b_open_file_;
  QPushButton *b_save_pic_;
  QPushButton *b_save_gif_;
  QString file_path_;
  QLineEdit *file_name_;
  QLabel *lb_figure_info_;

  QScrollBar *scb_set_size_;
  QSpinBox *spb_set_size_;
  QLabel *lb_scale_;

  QScrollBar *scb_move_x_;
  QSpinBox *spb_move_x_;
  QScrollBar *scb_move_y_;
  QSpinBox *spb_move_y_;
  QScrollBar *scb_move_z_;
  QSpinBox *spb_move_z_;
  QLabel *lb_move_;
  QLabel *lb_x_move_;
  QLabel *lb_y_move_;
  QLabel *lb_z_move_;

  QScrollBar *scb_rotate_x_;
  QSpinBox *spb_rotate_x_;
  QScrollBar *scb_rotate_y_;
  QSpinBox *spb_rotate_y_;
  QScrollBar *scb_rotate_z_;
  QSpinBox *spb_rotate_z_;
  QLabel *lb_rotate_;
  QLabel *lb_x_rotate_;
  QLabel *lb_y_rotate_;
  QLabel *lb_z_rotate_;

  QPushButton *b_reset_view_;
  QPushButton *b_parallel_proj_;
  QPushButton *b_central_proj_;
  QLabel *lb_proj_;

  QPushButton *b_settings_;
  QLabel *lb_copyright_;
  QLabel *lb_meme_;

  // GUI setup methods

  void setup_open_save_buttons();
  void setup_scale_buttons();
  void setup_move_buttons();
  void setup_rotate_buttons();
  void setup_reset_view_button();
  void setup_projection_buttons();
  void setup_settings_button();
  void reset_buttons();

  template <typename T>
  void set_value_silently(T *control, float value);

 signals:

 private slots:
  void slt_open_file_clicked();
  void slt_save_pic_clicked();
  void slt_save_gif_clicked();
  void get_gif_frame();

  void slt_scale_clicked(int value);

  void slt_move_x_clicked(float value);
  void slt_move_y_clicked(float value);
  void slt_move_z_clicked(float value);

  void slt_rotate_x_clicked(float value);
  void slt_rotate_y_clicked(float value);
  void slt_rotate_z_clicked(float value);

  void slt_reset_view_clicked();

  void slt_parallel_proj_clicked(bool checked);
  void slt_central_proj_clicked(bool checked);
  void slt_settings_clicked();
};
}  // namespace s21

#endif  // VIEW_H
