#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QDialog>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#include <QtWidgets>

#include "qt_scene_drawer.h"
#include "widget_utils.h"

namespace s21 {
/**
 * @class SettingsWidget
 * @brief A dialog widget that allows user to customize type, color and
 * thickness of the edges, display method, color and size of the vertices of a
 * 3D model.
 */
class SettingsWidget : public QDialog {
  Q_OBJECT
 public:
  explicit SettingsWidget(QWidget *parent = nullptr,
                          QtSceneDrawer::RendParams *params = nullptr);
  ~SettingsWidget();

 private:
  // Pointer to the rendering parameters struct in QtSceneDrawer object.
  QtSceneDrawer::RendParams *rend_params_;
  // Temporary rendering parameters struct for storing user's changes.
  // This struct is needed to avoid changing the original parameters when user
  // changed parameters, but clicked on "Cancel" button.
  QtSceneDrawer::RendParams *temp_rend_params_;

  // GUI elements and controls
  // b - button, spb - spinbox, lb - label

  QLabel *lb_background_;
  QPushButton *b_change_bg_color_;
  QLabel *lb_bg_color_sample_;

  QLabel *lb_edges_;
  QLabel *lb_edge_type_;
  QComboBox *cmb_change_edge_type_;
  QLabel *lb_edge_size_;
  QSpinBox *spb_change_edge_size_;
  QPushButton *b_change_edge_color_;
  QLabel *lb_edge_color_sample_;

  QLabel *lb_vertices_;
  QLabel *lb_vertex_type_;
  QComboBox *cmb_change_vertex_type_;
  QLabel *lb_vertex_size_;
  QSpinBox *spb_change_vertex_size_;
  QPushButton *b_change_vertex_color_;
  QLabel *lb_vertex_color_sample_;

  QPushButton *b_set_default_;
  QPushButton *b_ok_;
  QPushButton *b_cancel_;
  QDialogButtonBox *buttonbox_;

  // GUI setup methods

  void setup_bg_buttons();
  void setup_edges_buttons();
  void setup_vertices_buttons();
  void setup_dialog_buttons();
  void update_elems(QtSceneDrawer::RendParams *params);
  void copy_rend_params(QtSceneDrawer::RendParams *from,
                        QtSceneDrawer::RendParams *to);

 signals:

 private slots:
  void slt_change_bg_color_clicked();
  void slt_change_edge_color_clicked();
  void slt_change_vertex_color_clicked();
  void slt_change_edge_type_clicked(int index);
  void slt_change_edge_size_clicked(int value);
  void slt_change_vertex_type_clicked(int index);
  void slt_change_vertex_size_clicked(int value);
  void slt_set_default_clicked();
  void slt_ok_clicked();
  void slt_cancel_clicked();
};
}  // namespace s21

#endif  // SETTINGSWIDGET_H
