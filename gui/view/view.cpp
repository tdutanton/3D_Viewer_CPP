#include "view.h"

namespace s21 {
/**
 * @brief Constructor for View object.
 * The View class contains all user interface elements and visualisation area
 * for 3D scene.
 */
View::View(QWidget *parent, Controller *controller) : QWidget{parent} {
  controller_ = controller;
  drawer_ = new QtSceneDrawer(this);
  rend_params_ = drawer_->get_rend_params();
  view_settings_ = new SettingsWidget(this, drawer_->get_rend_params());
  update_timer_ = new QTimer(this);
  update_timer_->setSingleShot(true);
  gif_ = nullptr;
  record_gif_timer_ = new QTimer(this);
  gif_delay_timer_ = new QTimer(this);
  connect(gif_delay_timer_, &QTimer::timeout, this, &View::get_gif_frame);

  setup_open_save_buttons();
  setup_scale_buttons();
  setup_move_buttons();
  setup_rotate_buttons();
  setup_reset_view_button();
  setup_projection_buttons();
  setup_settings_button();
  reset_buttons();
}

/**
 * @brief Destructor for the View class.
 */
View::~View() {
  if (gif_ != nullptr) delete gif_;
}

/**
 * @brief Sets up the buttons for opening files, saving screenshots and saving
 * GIFs.
 */
void View::setup_open_save_buttons() {
  int start_x = DRAWER_WIDTH + DRAWER_INDENT + 20;
  int start_y = DRAWER_INDENT;
  QFont base_font("Arial", 11, QFont::Normal);
  QSize icon_size(25, 25);
  int bt_height = 30;
  int max_bt_width = 355;

  b_open_file_ =
      setup_button(this, "Open file", ":/img/images/folder_icon.png", icon_size,
                   start_x, start_y, 140, bt_height, base_font);
  connect(b_open_file_, &QPushButton::clicked, this,
          &View::slt_open_file_clicked);

  b_save_pic_ = setup_button(this, "Save screenshot",
                             ":/img/images/export_image_icon.png", icon_size,
                             b_open_file_->x() + b_open_file_->width() + 10,
                             start_y, 205, bt_height, base_font);
  connect(b_save_pic_, &QPushButton::clicked, this,
          &View::slt_save_pic_clicked);

  b_save_gif_ =
      setup_button(this, "Save gif", ":/img/images/export_gif_icon.png",
                   icon_size, start_x, start_y + b_open_file_->height() + 10,
                   max_bt_width, bt_height, base_font);
  connect(b_save_gif_, &QPushButton::clicked, this,
          &View::slt_save_gif_clicked);

  file_name_ = new QLineEdit(this);
  file_name_->setReadOnly(true);
  file_name_->setGeometry(
      start_x, b_save_gif_->y() + b_save_gif_->height() + 10, max_bt_width, 25);
  file_name_->setFont(base_font);
  file_name_->setAlignment(Qt::AlignCenter);

  lb_figure_info_ = setup_label(
      this, "", start_x, file_name_->y() + file_name_->height() + 5,
      max_bt_width, 25, QFont("Arial", 10, QFont::Normal), Qt::AlignCenter);
}

/**
 * @brief Sets up the scale controls (scrollbars, spinboxes)
 * for scaling 3D model.
 */
void View::setup_scale_buttons() {
  int start_x = DRAWER_WIDTH + DRAWER_INDENT + 20;
  int start_y = lb_figure_info_->y() + lb_figure_info_->height() + 15;
  QFont base_font("Arial", 11, QFont::Normal);
  int max_bt_width = 355;

  lb_scale_ = setup_label(this, "SCALE", start_x, start_y, max_bt_width, 25,
                          base_font, Qt::AlignCenter);

  scb_set_size_ = setup_scrollbar(this, Qt::Horizontal, start_x,
                                  lb_scale_->y() + lb_scale_->height() + 10,
                                  300, 20, SCALE_MIN, SCALE_MAX);
  connect(scb_set_size_, &QScrollBar::valueChanged, this,
          &View::slt_scale_clicked);

  spb_set_size_ =
      setup_spinbox(this, start_x + scb_set_size_->width() + 5,
                    scb_set_size_->y() - 1, 50, 22, SCALE_MIN, SCALE_MAX);
  connect(spb_set_size_, &QSpinBox::valueChanged, this,
          &View::slt_scale_clicked);
}

/**
 * @brief Sets up the move controls (scrollbars, spinboxes)
 * for moving 3D model.
 */
void View::setup_move_buttons() {
  int start_x = DRAWER_WIDTH + DRAWER_INDENT + 20;
  int start_y = scb_set_size_->y() + scb_set_size_->height() + 25;
  QFont base_font("Arial", 11, QFont::Normal);
  int max_bt_width = 355;

  lb_move_ = setup_label(this, "MOVE", start_x, start_y, max_bt_width, 25,
                         base_font, Qt::AlignCenter);

  lb_x_move_ =
      setup_label(this, "X", start_x, lb_move_->y() + lb_move_->height() + 10,
                  10, 22, base_font, Qt::AlignCenter);
  scb_move_x_ =
      setup_scrollbar(this, Qt::Horizontal, start_x + lb_x_move_->width() + 10,
                      lb_x_move_->y(), 280, 20, MOVE_MIN, MOVE_MAX);
  connect(scb_move_x_, &QScrollBar::valueChanged, this,
          &View::slt_move_x_clicked);
  spb_move_x_ = setup_spinbox(this, scb_move_x_->x() + scb_move_x_->width() + 5,
                              scb_move_x_->y() - 1, 50, 22, MOVE_MIN, MOVE_MAX);
  connect(spb_move_x_, &QSpinBox::valueChanged, this,
          &View::slt_move_x_clicked);

  lb_y_move_ = setup_label(this, "Y", start_x,
                           scb_move_x_->y() + scb_move_x_->height() + 10, 10,
                           22, base_font, Qt::AlignCenter);
  scb_move_y_ =
      setup_scrollbar(this, Qt::Horizontal, start_x + lb_y_move_->width() + 10,
                      lb_y_move_->y(), 280, 20, MOVE_MIN, MOVE_MAX);
  connect(scb_move_y_, &QScrollBar::valueChanged, this,
          &View::slt_move_y_clicked);
  spb_move_y_ = setup_spinbox(this, scb_move_y_->x() + scb_move_y_->width() + 5,
                              scb_move_y_->y() - 1, 50, 22, MOVE_MIN, MOVE_MAX);
  connect(spb_move_y_, &QSpinBox::valueChanged, this,
          &View::slt_move_y_clicked);

  lb_z_move_ = setup_label(this, "Z", start_x,
                           scb_move_y_->y() + scb_move_y_->height() + 10, 10,
                           22, base_font, Qt::AlignCenter);
  scb_move_z_ =
      setup_scrollbar(this, Qt::Horizontal, start_x + lb_z_move_->width() + 10,
                      lb_z_move_->y(), 280, 20, MOVE_MIN, MOVE_MAX);
  connect(scb_move_z_, &QScrollBar::valueChanged, this,
          &View::slt_move_z_clicked);
  spb_move_z_ = setup_spinbox(this, scb_move_z_->x() + scb_move_z_->width() + 5,
                              scb_move_z_->y() - 1, 50, 22, MOVE_MIN, MOVE_MAX);
  connect(spb_move_z_, &QSpinBox::valueChanged, this,
          &View::slt_move_z_clicked);
}

/**
 * @brief Sets up the rotate controls (scrollbars, spinboxes)
 * for rotating 3D model.
 */
void View::setup_rotate_buttons() {
  int start_x = DRAWER_WIDTH + DRAWER_INDENT + 20;
  int start_y = lb_z_move_->y() + lb_z_move_->height() + 25;
  QFont base_font("Arial", 11, QFont::Normal);
  int max_bt_width = 355;

  lb_rotate_ = setup_label(this, "ROTATE", start_x, start_y, max_bt_width, 25,
                           base_font, Qt::AlignCenter);

  lb_x_rotate_ = setup_label(this, "X", start_x,
                             lb_rotate_->y() + lb_rotate_->height() + 10, 10,
                             22, base_font, Qt::AlignCenter);
  scb_rotate_x_ = setup_scrollbar(
      this, Qt::Horizontal, start_x + lb_x_rotate_->width() + 10,
      lb_x_rotate_->y(), 280, 20, ROTATE_MIN, ROTATE_MAX);
  connect(scb_rotate_x_, &QScrollBar::valueChanged, this,
          &View::slt_rotate_x_clicked);
  spb_rotate_x_ =
      setup_spinbox(this, scb_rotate_x_->x() + scb_rotate_x_->width() + 5,
                    scb_rotate_x_->y() - 1, 50, 22, ROTATE_MIN, ROTATE_MAX);
  connect(spb_rotate_x_, &QSpinBox::valueChanged, this,
          &View::slt_rotate_x_clicked);

  lb_y_rotate_ = setup_label(this, "Y", start_x,
                             spb_rotate_x_->y() + spb_rotate_x_->height() + 10,
                             10, 22, base_font, Qt::AlignCenter);
  scb_rotate_y_ = setup_scrollbar(
      this, Qt::Horizontal, lb_y_rotate_->x() + lb_y_rotate_->width() + 10,
      lb_y_rotate_->y(), 280, 20, ROTATE_MIN, ROTATE_MAX);
  connect(scb_rotate_y_, &QScrollBar::valueChanged, this,
          &View::slt_rotate_y_clicked);
  spb_rotate_y_ =
      setup_spinbox(this, scb_rotate_y_->x() + scb_rotate_y_->width() + 5,
                    scb_rotate_y_->y() - 1, 50, 22, ROTATE_MIN, ROTATE_MAX);
  connect(spb_rotate_y_, &QSpinBox::valueChanged, this,
          &View::slt_rotate_y_clicked);

  lb_z_rotate_ = setup_label(this, "Z", start_x,
                             scb_rotate_y_->y() + scb_rotate_y_->height() + 10,
                             10, 22, base_font, Qt::AlignCenter);
  scb_rotate_z_ = setup_scrollbar(
      this, Qt::Horizontal, lb_z_rotate_->x() + lb_z_rotate_->width() + 10,
      lb_z_rotate_->y(), 280, 20, ROTATE_MIN, ROTATE_MAX);
  connect(scb_rotate_z_, &QScrollBar::valueChanged, this,
          &View::slt_rotate_z_clicked);
  spb_rotate_z_ =
      setup_spinbox(this, scb_rotate_z_->x() + scb_rotate_z_->width() + 5,
                    scb_rotate_z_->y() - 1, 50, 22, ROTATE_MIN, ROTATE_MAX);
  connect(spb_rotate_z_, &QSpinBox::valueChanged, this,
          &View::slt_rotate_z_clicked);
}

/**
 * @brief Sets up the reset button for 3D model. This button allows user
 * reset position of the 3D model to its default state.
 */
void View::setup_reset_view_button() {
  int start_x = DRAWER_WIDTH + DRAWER_INDENT + 20;
  int start_y = scb_rotate_z_->y() + scb_rotate_z_->height() + 25;
  QFont base_font("Arial", 11, QFont::Normal);
  QSize icon_size(21, 21);
  int bt_height = 30;
  int max_bt_width = 355;

  b_reset_view_ = setup_button(
      this, "  Reset position", ":/img/images/reset_icon.png", icon_size,
      start_x, start_y, max_bt_width, bt_height, base_font);
  connect(b_reset_view_, &QPushButton::clicked, this,
          &View::slt_reset_view_clicked);
}

/**
 * @brief Sets up projection buttons for the 3D model. This buttons allow user
 * to switch between parallel and central projection modes for the 3D model.
 */
void View::setup_projection_buttons() {
  int start_x = DRAWER_WIDTH + DRAWER_INDENT + 20;
  int start_y = b_reset_view_->y() + b_reset_view_->height() + 25;
  QFont base_font("Arial", 11, QFont::Normal);
  int bt_height = 30;
  int max_bt_width = 355;

  lb_proj_ = setup_label(this, "PROJECTION", start_x, start_y, max_bt_width,
                         bt_height, base_font, Qt::AlignCenter);

  b_parallel_proj_ = setup_button(this, "Parallel", "", QSize(), lb_proj_->x(),
                                  lb_proj_->y() + lb_proj_->height() + 10,
                                  max_bt_width / 2, bt_height, base_font);
  b_parallel_proj_->setCheckable(true);
  if (rend_params_->projection == QtSceneDrawer::RendParams::PARALLEL)
    b_parallel_proj_->setChecked(true);
  connect(b_parallel_proj_, &QPushButton::toggled, this,
          &View::slt_parallel_proj_clicked);

  b_central_proj_ = setup_button(
      this, "Central", "", QSize(), lb_proj_->x() + b_parallel_proj_->width(),
      b_parallel_proj_->y(), max_bt_width / 2, bt_height, base_font);
  b_central_proj_->setCheckable(true);
  if (rend_params_->projection == QtSceneDrawer::RendParams::CENTRAL)
    b_central_proj_->setChecked(true);
  connect(b_central_proj_, &QPushButton::toggled, this,
          &View::slt_central_proj_clicked);
}

/**
 * @brief Sets up settings button, which allows user allow setting up the type
 * (solid, dashed), color and thickness of the edges, display method (none,
 * circle, square), color and size of the vertices.
 */
void View::setup_settings_button() {
  int start_x = DRAWER_WIDTH + DRAWER_INDENT + 20;
  int start_y = b_parallel_proj_->y() + b_parallel_proj_->height() + 25;
  QFont base_font("Arial", 11, QFont::Normal);
  QSize icon_size(25, 25);
  int bt_height = 40;
  int max_bt_width = 355;

  b_settings_ = setup_button(
      this, " View settings", ":/img/images/settings_icon.png", icon_size,
      start_x, start_y, max_bt_width, bt_height, base_font);
  connect(b_settings_, &QPushButton::clicked, this,
          &View::slt_settings_clicked);

  lb_copyright_ = setup_label(this, "3DViewer © \nmontoyay \nbuggkell", start_x,
                              b_settings_->y() + b_settings_->height() + 30,
                              260, 60, base_font, Qt::AlignRight);
  QPixmap pixmap(":/img/images/3dcat.png");
  lb_meme_ = new QLabel(this);
  lb_meme_->setPixmap(pixmap.scaled(70, 70));
  lb_meme_->move(QPoint(lb_copyright_->x() + 90, lb_copyright_->y() - 10));
}

/**
 * @brief Handles the scaling of the 3D model when the scale slider or spinbox
 * value is changed.
 * @param value The new scale value from the slider or spinbox.
 */
void View::slt_scale_clicked(int value) {
  if (!update_timer_->isActive()) {
    update_timer_->start(8);

    float new_scale = value / 100.0f;
    controller_->scale_model(1.0f / prev_pos_.prev_scale * new_scale);

    prev_pos_.prev_scale = new_scale;
    drawer_->DrawFigure(controller_->get_figure());
    drawer_->update();

    set_value_silently(scb_set_size_, value);
    set_value_silently(spb_set_size_, value);
  } else {
    set_value_silently(scb_set_size_, prev_pos_.prev_scale * 100);
    set_value_silently(spb_set_size_, prev_pos_.prev_scale * 100);
  }
}

/**
 * @brief Handles opening a 3D model file in the OBJ format.
 */
void View::slt_open_file_clicked() {
  file_path_ =
      QFileDialog::getOpenFileName(this, "Open file", "", "OBJ (*.obj)");

  if (!file_path_.isEmpty()) {
    controller_->read_model(file_path_.toStdString());
    QFileInfo file_info(file_path_);
    file_name_->setText(file_info.fileName());
    QLocale locale;
    std::vector<int> model_data(controller_->get_model_data());
    lb_figure_info_->setText("Vertices: " + locale.toString(model_data[0]) +
                             ", edges: " + locale.toString(model_data[1]) +
                             " (" + locale.toString(model_data[2]) + ")");
    controller_->scale_model(0.98);
    reset_buttons();
    prev_pos_ = PrevPositions{};
  }
  drawer_->makeCurrent();
  drawer_->DrawFigure(controller_->get_figure());
  drawer_->update();
}

/**
 * @brief Saves a screenshot of the wireframe model as an image file.
 */
void View::slt_save_pic_clicked() {
  QPixmap pic = drawer_->grab(QRect(0, 0, drawer_->width(), drawer_->height()));
  QString filters("BMP (*.bmp);;JPEG (*.jpeg)");
  QString set_filter;
  QString file_name_ =
      "3DViewer_screen_" +
      QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss");
  QString file_path_ = QFileDialog::getSaveFileName(
      this, "Save picture", file_name_, filters, &set_filter);
  if (!file_path_.isEmpty()) {
    if (set_filter.contains("bmp")) {
      if (!file_path_.endsWith(".bmp", Qt::CaseInsensitive))
        file_path_ = file_path_ + ".bmp";
      pic.save(file_path_, "BMP");
    }
    if (set_filter.contains("jpeg")) {
      if (!file_path_.endsWith(".jpeg", Qt::CaseInsensitive))
        file_path_ = file_path_ + ".jpeg";
      pic.save(file_path_, "JPEG");
    }
  }
}

/**
 * @brief Starts recording a GIF animation of the current wireframe model view.
 *
 * Сalled when user clicks the "Save GIF" button. It creates a new QGifImage
 * object, sets the default delay between frames, and starts the GIF recording
 * timer.
 */
void View::slt_save_gif_clicked() {
  if (record_gif_timer_->isActive()) return;

  if (gif_ != nullptr) {
    delete gif_;
  }
  gif_ = new QGifImage();

  gif_->setDefaultDelay(100);
  record_gif_timer_->setSingleShot(true);

  b_save_gif_->setIcon(QIcon(":/img/images/recording_icon.png"));
  b_save_gif_->setIconSize(QSize(23, 23));
  b_save_gif_->setText("  Recording ...");
  gif_delay_timer_->start(100);
  record_gif_timer_->start(5000);
}

/**
 * @brief Get frame from current 3D model view and adds it to the GIF animation.
 *
 * This function is called repeatedly by a timer to capture frames for the GIF
 * animation. While recording timer is active, it grabs current frame from
 * visualisation area and adds it to the GIF image. When the recording timer
 * expires, it stops timers, suggests user to save the GIF.
 */
void View::get_gif_frame() {
  if (record_gif_timer_->isActive()) {
    QImage frame = drawer_->grabFramebuffer();
    gif_->addFrame(frame);
  } else {
    gif_delay_timer_->stop();
    record_gif_timer_->stop();
    QString set_filter;
    QString file_name_ =
        "3DViewer_screen_" +
        QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss");
    QString file_path_ = QFileDialog::getSaveFileName(
        this, "Save gif", file_name_, "GIF (*.gif)", &set_filter);

    if (!file_path_.isEmpty()) {
      if (!file_path_.endsWith(".gif", Qt::CaseInsensitive))
        file_path_ = file_path_ + ".gif";
      gif_->save(file_path_);
    }
    delete gif_;
    gif_ = nullptr;
    b_save_gif_->setIcon(QIcon(":/img/images/export_gif_icon.png"));
    b_save_gif_->setIconSize(QSize(25, 25));
    b_save_gif_->setText("Save gif");
  }
}

/**
 * @brief Handles moving x-axis of the 3D model when move slider or spinbox
 * value is changed.
 * @param value The new move value from slider or spinbox.
 */
void View::slt_move_x_clicked(float value) {
  if (!update_timer_->isActive()) {
    update_timer_->start(8);
    float new_pos = value / 100.0f;
    controller_->move_model(-prev_pos_.prev_x_pos + new_pos, 0, 0);

    drawer_->DrawFigure(controller_->get_figure());
    drawer_->update();

    set_value_silently(scb_move_x_, value);
    set_value_silently(spb_move_x_, value);

    prev_pos_.prev_x_pos = new_pos;

  } else {
    set_value_silently(scb_move_x_, prev_pos_.prev_x_pos * 100.0f);
    set_value_silently(spb_move_x_, prev_pos_.prev_x_pos * 100.0f);
  }
}

/**
 * @brief Handles moving y-axis of the 3D model when move slider or spinbox
 * value is changed.
 * @param value The new move value from slider or spinbox.
 */
void View::slt_move_y_clicked(float value) {
  if (!update_timer_->isActive()) {
    update_timer_->start(8);
    float new_pos = value / 100.0f;
    controller_->move_model(0, -prev_pos_.prev_y_pos + new_pos, 0);

    drawer_->DrawFigure(controller_->get_figure());
    drawer_->update();

    set_value_silently(scb_move_y_, value);
    set_value_silently(spb_move_y_, value);

    prev_pos_.prev_y_pos = new_pos;
  } else {
    set_value_silently(scb_move_y_, prev_pos_.prev_y_pos * 100.0f);
    set_value_silently(spb_move_y_, prev_pos_.prev_y_pos * 100.0f);
  }
}

/**
 * @brief Handles moving y-axis of the 3D model when move slider or spinbox
 * value is changed.
 * @param value The new move value from slider or spinbox.
 */
void View::slt_move_z_clicked(float value) {
  if (!update_timer_->isActive()) {
    update_timer_->start(8);
    float new_pos = value / 100.0f;
    controller_->move_model(0, 0, -prev_pos_.prev_z_pos + new_pos);

    drawer_->DrawFigure(controller_->get_figure());
    drawer_->update();

    set_value_silently(scb_move_z_, value);
    set_value_silently(spb_move_z_, value);

    prev_pos_.prev_z_pos = new_pos;
  } else {
    set_value_silently(scb_move_z_, prev_pos_.prev_z_pos * 100.0f);
    set_value_silently(spb_move_z_, prev_pos_.prev_z_pos * 100.0f);
  }
}

/**
 * @brief Handles the rotating the x-axis of the 3D model when rotate slider or
 * spinbox value is changed.
 * @param value The new rotate angle from slider or spinbox.
 */
void View::slt_rotate_x_clicked(float value) {
  if (!update_timer_->isActive()) {
    update_timer_->start(8);
    controller_->rotate_model(value - prev_pos_.prev_x_angle, 0, 0);
    drawer_->DrawFigure(controller_->get_figure());
    drawer_->update();

    set_value_silently(scb_rotate_x_, value);
    set_value_silently(spb_rotate_x_, value);

    prev_pos_.prev_x_angle = value;
  } else {
    set_value_silently(scb_rotate_x_, prev_pos_.prev_x_angle);
    set_value_silently(spb_rotate_x_, prev_pos_.prev_x_angle);
  }
}

/**
 * @brief Handles the rotating the y-axis of the 3D model when rotate slider or
 * spinbox value is changed.
 * @param value The new rotate angle from slider or spinbox.
 */
void View::slt_rotate_y_clicked(float value) {
  if (!update_timer_->isActive()) {
    update_timer_->start(8);
    controller_->rotate_model(0, value - prev_pos_.prev_y_angle, 0);
    drawer_->DrawFigure(controller_->get_figure());
    drawer_->update();

    set_value_silently(scb_rotate_y_, value);
    set_value_silently(spb_rotate_y_, value);

    prev_pos_.prev_y_angle = value;
  } else {
    set_value_silently(scb_rotate_y_, prev_pos_.prev_y_angle);
    set_value_silently(spb_rotate_y_, prev_pos_.prev_y_angle);
  }
}

/**
 * @brief Handles the rotating the z-axis of the 3D model when rotate slider or
 * spinbox value is changed.
 * @param value The new rotate angle from slider or spinbox.
 */
void View::slt_rotate_z_clicked(float value) {
  if (!update_timer_->isActive()) {
    update_timer_->start(8);
    controller_->rotate_model(0, 0, value - prev_pos_.prev_z_angle);
    drawer_->DrawFigure(controller_->get_figure());
    drawer_->update();

    set_value_silently(scb_rotate_z_, value);
    set_value_silently(spb_rotate_z_, value);

    prev_pos_.prev_z_angle = value;
  } else {
    set_value_silently(scb_rotate_z_, prev_pos_.prev_z_angle);
    set_value_silently(spb_rotate_z_, prev_pos_.prev_z_angle);
  }
}

/**
 * @brief Resets view of the 3D model to its default state.
 *
 * This function resets position, scale, and rotation of the 3D model to their
 * initial values, and updates the UI controls accordingly.
 */
void View::slt_reset_view_clicked() {
  prev_pos_ = PrevPositions{};
  controller_->reset_model_pos();
  controller_->scale_model(0.98);
  drawer_->DrawFigure(controller_->get_figure());
  drawer_->update();
  reset_buttons();
}

/**
 * @brief Handles switch central projection of 3d model to parallel projection.
 */
void View::slt_parallel_proj_clicked(bool checked) {
  if (checked) {
    b_central_proj_->blockSignals(true);
    b_central_proj_->setChecked(false);
    b_central_proj_->blockSignals(false);
    rend_params_->projection = QtSceneDrawer::RendParams::PARALLEL;
    drawer_->update();
  }
  b_parallel_proj_->setChecked(true);
}

/**
 * @brief Handles switch parallel projection of 3d model to central projection.
 */
void View::slt_central_proj_clicked(bool checked) {
  if (checked) {
    b_parallel_proj_->blockSignals(true);
    b_parallel_proj_->setChecked(false);
    b_parallel_proj_->blockSignals(false);
    rend_params_->projection = QtSceneDrawer::RendParams::CENTRAL;
    drawer_->update();
  }
  b_central_proj_->setChecked(true);
}

/**
 * @brief Opens view settings dialog window.
 */
void View::slt_settings_clicked() {
  view_settings_->exec();
  drawer_->update();
}

/**
 * @brief Resets all scaling, moving and rotating 3d model controls values to
 * their initial values.
 */
void View::reset_buttons() {
  set_value_silently(spb_set_size_, START_SCALE);
  set_value_silently(scb_set_size_, START_SCALE);

  set_value_silently(scb_move_x_, START_POS);
  set_value_silently(spb_move_x_, START_POS);
  set_value_silently(scb_move_y_, START_POS);
  set_value_silently(spb_move_y_, START_POS);
  set_value_silently(scb_move_z_, START_POS);
  set_value_silently(spb_move_z_, START_POS);

  set_value_silently(scb_rotate_x_, START_ANGLE);
  set_value_silently(spb_rotate_x_, START_ANGLE);
  set_value_silently(scb_rotate_y_, START_ANGLE);
  set_value_silently(spb_rotate_y_, START_ANGLE);
  set_value_silently(scb_rotate_z_, START_ANGLE);
  set_value_silently(spb_rotate_z_, START_ANGLE);
}

/**
 * @brief Sets the value of a control silently, without triggering any signals.
 *
 * @param control The control whose value should be set.
 * @param value The new value to be set.
 */
template <typename T>
void View::set_value_silently(T *control, float value) {
  control->blockSignals(true);
  control->setValue(value);
  control->blockSignals(false);
}

}  // namespace s21
