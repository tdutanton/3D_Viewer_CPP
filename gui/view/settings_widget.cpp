#include "settings_widget.h"

namespace s21 {
/**
 * @brief Constructor for SettingsWidget object.
 *
 * @param parent The parent widget for the SettingsWidget.
 * @param params A pointer to the rendering parameters struct in QtSceneDrawer
 */
SettingsWidget::SettingsWidget(QWidget *parent,
                               QtSceneDrawer::RendParams *params)
    : QDialog(parent) {
  rend_params_ = params;
  temp_rend_params_ = new QtSceneDrawer::RendParams();
  copy_rend_params(rend_params_, temp_rend_params_);
  this->setModal(true);
  this->setWindowTitle("View settings");
  this->setWindowIcon(QIcon(":/img/images/settings_icon.png"));
  this->setFixedSize(440, 380);
  setup_bg_buttons();
  setup_edges_buttons();
  setup_vertices_buttons();
  setup_dialog_buttons();
}

/**
 * @brief Destructor for SettingsWidget object. Deletes temporary rendering
 * parameters struct.
 */
SettingsWidget::~SettingsWidget() { delete temp_rend_params_; }

/**
 * @brief Creates buttons and labels for setting up background collor.
 */
void SettingsWidget::setup_bg_buttons() {
  int start_x = this->x() + 10;
  int start_y = this->y() + 10;
  int bt_height = 30;
  QFont base_font("Arial", 11, QFont::Normal);
  Qt::Alignment align = Qt::AlignCenter;

  lb_background_ = setup_label(this, "BACKGROUND", start_x, start_y,
                               this->width() - 20, 25, base_font, align);
  b_change_bg_color_ =
      setup_button(this, "Change color ...", "", QSize(), start_x,
                   lb_background_->y() + lb_background_->height() + 10, 380,
                   bt_height, base_font);
  connect(b_change_bg_color_, &QPushButton::clicked, this,
          &SettingsWidget::slt_change_bg_color_clicked);
  lb_bg_color_sample_ = setup_label(
      this, "", b_change_bg_color_->x() + b_change_bg_color_->width() + 10,
      b_change_bg_color_->y() + 1, bt_height - 2, bt_height - 2, base_font,
      align);
  lb_bg_color_sample_->setFrameStyle(QFrame::Box | QFrame::Plain);
  lb_bg_color_sample_->setAutoFillBackground(true);
  lb_bg_color_sample_->setPalette(QPalette(rend_params_->bg_color));
}

/**
 * @brief Creates control elements for setting up edges collor, thickness and
 * type.
 */
void SettingsWidget::setup_edges_buttons() {
  int start_x = this->x() + 10;
  int bt_height = 30;
  QFont base_font("Arial", 11, QFont::Normal);
  QFont small_font("Arial", 10, QFont::Normal);
  QStringList edge_type_list = {"None", "Solid", "Dashed"};
  Qt::Alignment align = Qt::AlignCenter;

  lb_edges_ =
      setup_label(this, "EDGES", start_x,
                  b_change_bg_color_->y() + b_change_bg_color_->height() + 30,
                  this->width() - 20, 25, base_font, align);
  lb_edge_type_ = setup_label(this, "Type", start_x,
                              lb_edges_->y() + lb_edges_->height() + 5, 100, 15,
                              small_font, align);
  cmb_change_edge_type_ = setup_combobox(
      this, edge_type_list, start_x, lb_edges_->y() + lb_edges_->height() + 25,
      100, bt_height, base_font);
  cmb_change_edge_type_->setCurrentIndex(rend_params_->edge_type);
  connect(cmb_change_edge_type_, &QComboBox::currentIndexChanged, this,
          &SettingsWidget::slt_change_edge_type_clicked);
  lb_edge_size_ = setup_label(this, "Size",
                              lb_edge_type_->x() + lb_edge_type_->width() + 10,
                              lb_edge_type_->y(), 60, 15, small_font, align);
  spb_change_edge_size_ =
      setup_spinbox(this, start_x + cmb_change_edge_type_->width() + 10,
                    cmb_change_edge_type_->y(), 60, bt_height, 1, 10);
  spb_change_edge_size_->setValue(rend_params_->edge_thickness);
  connect(spb_change_edge_size_, &QSpinBox::valueChanged, this,
          &SettingsWidget::slt_change_edge_size_clicked);
  b_change_edge_color_ = setup_button(
      this, "Change color ...", "", QSize(),
      spb_change_edge_size_->x() + spb_change_edge_size_->width() + 10,
      spb_change_edge_size_->y(), 200, bt_height, base_font);
  connect(b_change_edge_color_, &QPushButton::clicked, this,
          &SettingsWidget::slt_change_edge_color_clicked);
  lb_edge_color_sample_ = setup_label(
      this, "", b_change_edge_color_->x() + b_change_edge_color_->width() + 10,
      b_change_edge_color_->y() + 1, bt_height - 2, bt_height - 2, base_font,
      align);
  lb_edge_color_sample_->setFrameStyle(QFrame::Box | QFrame::Plain);
  lb_edge_color_sample_->setAutoFillBackground(true);
  lb_edge_color_sample_->setPalette(QPalette(rend_params_->edge_color));
}

/**
 * @brief Creates control elements for setting up vertices collor, thickness and
 * type.
 */
void SettingsWidget::setup_vertices_buttons() {
  int start_x = this->x() + 10;
  int bt_height = 30;
  QFont base_font("Arial", 11, QFont::Normal);
  QFont small_font("Arial", 10, QFont::Normal);
  QStringList vertex_type_list = {"None", "Circle", "Square"};
  Qt::Alignment align = Qt::AlignCenter;

  lb_vertices_ = setup_label(
      this, "VERTICES", start_x,
      cmb_change_edge_type_->y() + cmb_change_edge_type_->height() + 30,
      this->width() - 20, 25, base_font, align);
  lb_vertex_type_ = setup_label(this, "Type", start_x,
                                lb_vertices_->y() + lb_vertices_->height() + 5,
                                100, 15, small_font, align);
  cmb_change_vertex_type_ =
      setup_combobox(this, vertex_type_list, start_x,
                     lb_vertices_->y() + lb_vertices_->height() + 25, 100,
                     bt_height, base_font);
  cmb_change_vertex_type_->setCurrentIndex(rend_params_->vertex_type);
  connect(cmb_change_vertex_type_, &QComboBox::currentIndexChanged, this,
          &SettingsWidget::slt_change_vertex_type_clicked);
  lb_vertex_size_ = setup_label(
      this, "Size", lb_vertex_type_->x() + lb_vertex_type_->width() + 10,
      lb_vertex_type_->y(), 60, 15, small_font, align);
  spb_change_vertex_size_ =
      setup_spinbox(this, start_x + cmb_change_vertex_type_->width() + 10,
                    cmb_change_vertex_type_->y(), 60, bt_height, 1, 20);
  spb_change_vertex_size_->setValue(rend_params_->vertex_size);
  connect(spb_change_vertex_size_, &QSpinBox::valueChanged, this,
          &SettingsWidget::slt_change_vertex_size_clicked);
  b_change_vertex_color_ = setup_button(
      this, "Change color ...", "", QSize(),
      spb_change_vertex_size_->x() + spb_change_vertex_size_->width() + 10,
      spb_change_vertex_size_->y(), 200, bt_height, base_font);
  connect(b_change_vertex_color_, &QPushButton::clicked, this,
          &SettingsWidget::slt_change_vertex_color_clicked);
  lb_vertex_color_sample_ = setup_label(
      this, "",
      b_change_vertex_color_->x() + b_change_vertex_color_->width() + 10,
      b_change_vertex_color_->y() + 1, bt_height - 2, bt_height - 2, base_font,
      align);
  lb_vertex_color_sample_->setFrameStyle(QFrame::Box | QFrame::Plain);
  lb_vertex_color_sample_->setAutoFillBackground(true);
  lb_vertex_color_sample_->setPalette(QPalette(rend_params_->vertex_color));
}

/**
 * @brief Creates dialog window buttons for saving, canceling changes and
 * restore defaults.
 */
void SettingsWidget::setup_dialog_buttons() {
  buttonbox_ =
      new QDialogButtonBox(QDialogButtonBox::RestoreDefaults |
                               QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           this);
  buttonbox_->move(125, spb_change_vertex_size_->y() + 70);

  b_set_default_ = buttonbox_->button(QDialogButtonBox::RestoreDefaults);
  connect(b_set_default_, &QPushButton::clicked, this,
          &SettingsWidget::slt_set_default_clicked);
  b_ok_ = buttonbox_->button(QDialogButtonBox::Ok);
  connect(b_ok_, &QPushButton::clicked, this, &SettingsWidget::slt_ok_clicked);
  b_cancel_ = buttonbox_->button(QDialogButtonBox::Cancel);
  connect(b_cancel_, &QPushButton::clicked, this,
          &SettingsWidget::slt_cancel_clicked);
}

/**
 * @brief Updates the UI elements in the settings widget to reflect the provided
 * rendering parameters.
 *
 * This function is used to synchronize the UI elements with the selected
 * rendering parameters.
 *
 * @param params The rendering parameters to update the UI with.
 */
void SettingsWidget::update_elems(QtSceneDrawer::RendParams *params) {
  lb_bg_color_sample_->setPalette(QPalette(params->bg_color));
  lb_edge_color_sample_->setPalette(QPalette(params->edge_color));
  lb_vertex_color_sample_->setPalette(QPalette(params->vertex_color));

  cmb_change_edge_type_->setCurrentIndex(params->edge_type);
  cmb_change_vertex_type_->setCurrentIndex(params->vertex_type);

  spb_change_edge_size_->setValue(params->edge_thickness);
  spb_change_vertex_size_->setValue(params->vertex_size);
}

/**
 * @brief Copies the rendering parameters from one RendParams struct to another.
 *
 * This function is needed for quick exchange between two RendParams structs.
 *
 * @param from The RendParams object to copy the parameters from.
 * @param to The RendParams object to copy the parameters to.
 */
void SettingsWidget::copy_rend_params(QtSceneDrawer::RendParams *from,
                                      QtSceneDrawer::RendParams *to) {
  to->bg_color = from->bg_color;
  to->edge_color = from->edge_color;
  to->edge_thickness = from->edge_thickness;
  to->edge_type = from->edge_type;
  to->vertex_color = from->vertex_color;
  to->vertex_size = from->vertex_size;
  to->vertex_type = from->vertex_type;
}

/**
 * @brief Handles click event for changing background color.
 */
void SettingsWidget::slt_change_bg_color_clicked() {
  QColor prev_color = temp_rend_params_->bg_color;
  temp_rend_params_->bg_color = QColorDialog::getColor(
      temp_rend_params_->bg_color, this, "Background color");
  if (!temp_rend_params_->bg_color.isValid())
    temp_rend_params_->bg_color = prev_color;
  lb_bg_color_sample_->setPalette(QPalette(temp_rend_params_->bg_color));
}

/**
 * @brief Handles click event for changing edges color.
 */
void SettingsWidget::slt_change_edge_color_clicked() {
  QColor prev_color = temp_rend_params_->edge_color;
  temp_rend_params_->edge_color =
      QColorDialog::getColor(temp_rend_params_->edge_color, this, "Edge color");
  if (!temp_rend_params_->edge_color.isValid())
    temp_rend_params_->edge_color = prev_color;
  lb_edge_color_sample_->setPalette(QPalette(temp_rend_params_->edge_color));
}

/**
 * @brief Handles click event for changing vertices color.
 */
void SettingsWidget::slt_change_vertex_color_clicked() {
  QColor prev_color = temp_rend_params_->vertex_color;
  temp_rend_params_->vertex_color = QColorDialog::getColor(
      temp_rend_params_->vertex_color, this, "Vertex color");
  if (!temp_rend_params_->vertex_color.isValid())
    temp_rend_params_->vertex_color = prev_color;
  lb_vertex_color_sample_->setPalette(
      QPalette(temp_rend_params_->vertex_color));
}

/**
 * @brief Handles click event for changing type of edges.
 * @param index The index of the selected edge type {"None", "Solid", "Dashed"}.
 */
void SettingsWidget::slt_change_edge_type_clicked(int index) {
  temp_rend_params_->edge_type = (QtSceneDrawer::RendParams::EdgesType)index;
}

/**
 * @brief Handles click event for changing the thickness of edges.
 * @param value The new thickness value for the edges.
 */
void SettingsWidget::slt_change_edge_size_clicked(int value) {
  temp_rend_params_->edge_thickness = value;
}

/**
 * @brief Handles click event for changing the type of vertices.
 * @param index The index of the selected vertex type {"None", "Circle",
 * "Square"}.
 */
void SettingsWidget::slt_change_vertex_type_clicked(int index) {
  temp_rend_params_->vertex_type =
      (QtSceneDrawer::RendParams::VerticesType)index;
}

/**
 * @brief Handles click event for changing the size of vertices.
 * @param value The new size value for vertices.
 */
void SettingsWidget::slt_change_vertex_size_clicked(int value) {
  temp_rend_params_->vertex_size = value;
}

/**
 * @brief Resets the rendering parameters to their default values.
 *
 * This function is called when the user clicks the "Set Default" button in the
 * settings widget. It deletes the current temporary rendering parameters
 * object, creates a new one with default values, and updates the UI elements
 * to reflect the new settings.
 */
void SettingsWidget::slt_set_default_clicked() {
  if (temp_rend_params_ != nullptr) {
    delete temp_rend_params_;
    temp_rend_params_ = nullptr;
  }
  temp_rend_params_ = new QtSceneDrawer::RendParams();
  update_elems(temp_rend_params_);
}

/**
 * @brief Handles the click event for the "OK" button.
 *
 * This function copies the temporary rendering parameters to the main rendering
 * parameters object.
 */
void SettingsWidget::slt_ok_clicked() {
  copy_rend_params(temp_rend_params_, rend_params_);
  accept();
}

/**
 * @brief Handles the click event for the "Cancel" button.
 *
 * This function copies the main rendering parameters back to the temporary
 * rendering parameters object, updates the UI elements to reflect the main
 * rendering parameters.
 */
void SettingsWidget::slt_cancel_clicked() {
  copy_rend_params(rend_params_, temp_rend_params_);
  update_elems(rend_params_);
  reject();
}
}  // namespace s21
