#include "qt_scene_drawer.h"

namespace s21 {

/**
 * @brief Constructor for QtSceneDrawer object. *
 * @param parent The parent QWidget for this QtSceneDrawer.
 */
QtSceneDrawer::QtSceneDrawer(QWidget* parent)
    : QOpenGLWidget(parent),
      view_settings_("montoyay_buggkell", "3DViewerCPP") {
  this->setFixedSize(DRAWER_WIDTH, DRAWER_HEIGHT);
  this->move(DRAWER_INDENT, DRAWER_INDENT);
  rend_params_ = new RendParams();
  load_view_settings();
  edges_array_ = nullptr;
}

/**
 * @brief Destructor for the QtSceneDrawer object.
 */
QtSceneDrawer::~QtSceneDrawer() {
  save_view_settings();
  if (rend_params_) delete rend_params_;
  if (edges_array_ != nullptr) {
    delete[] edges_array_;
    edges_array_ = nullptr;
  }
}

/**
 * @brief Initializes the OpenGL functions. This function called automatically
 * when the OpenGL context is created.
 */
void QtSceneDrawer::initializeGL() {
  initializeOpenGLFunctions();
  makeCurrent();
  glEnable(GL_DEPTH_TEST);
}

/**
 * @brief Resizes the OpenGL area to the specified width and height.
 * Called automatically when widget is resized.
 * @param w The new width of widget.
 * @param h The new height of widget.
 */
void QtSceneDrawer::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

/**
 * @brief Renders the 3D model using the current rendering parameters.
 * This function is called automatically when widget is updated.
 */
void QtSceneDrawer::paintGL() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Setting up model projection
  if (rend_params_->projection == RendParams::PARALLEL)
    glOrtho(-1, 1, -1, 1, 1, 1000);
  else
    glFrustum(-1, 1, -1, 1, 1, 1000);

  // Setting up background color
  glClearColor(rend_params_->bg_color.redF(), rend_params_->bg_color.greenF(),
               rend_params_->bg_color.blueF(), rend_params_->bg_color.alpha());

  // Clearing the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // Setting up model view "box"
  glTranslatef(0, 0, -2);
  // Setting the vertex array for rendering.
  glVertexPointer(3, GL_FLOAT, 0, edges_array_);
  glEnableClientState(GL_VERTEX_ARRAY);

  // Setting lines thickness for drawing edges
  glLineWidth(rend_params_->edge_thickness);

  if (rend_params_->edge_type != RendParams::NO_EDGES) {
    // Setting color for drawing edge
    glColor3f(rend_params_->edge_color.redF(),
              rend_params_->edge_color.greenF(),
              rend_params_->edge_color.blueF());

    if (rend_params_->edge_type == RendParams::DASHED) {
      // Setting the line stipple pattern
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x00FF);
    }
    // Drawing lines with 2 vertices
    glDrawArrays(GL_LINES, 0, array_size_ / 3);
    glDisable(GL_LINE_STIPPLE);
  }

  if (rend_params_->vertex_type != RendParams::NO_VERTICES) {
    // Setting color for drawing vertex
    glColor3f(rend_params_->vertex_color.redF(),
              rend_params_->vertex_color.greenF(),
              rend_params_->vertex_color.blueF());

    if (rend_params_->vertex_type == RendParams::CIRCLE) {
      // Setting the vertices circle form
      glEnable(GL_POINT_SMOOTH);
    }
    // Setting the vertices size
    glPointSize(rend_params_->vertex_size);
    // Drawing vertices
    glDrawArrays(GL_POINTS, 0, array_size_ / 3);
    glDisable(GL_POINT_SMOOTH);
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}

/**
 * @brief Returns a pointer to the RendParams object used by the QtSceneDrawer.
 *
 * Simple getter function for delivering render parameters to View and
 * SettingsWidget objects.
 *
 * @return QtSceneDrawer::RendParams* A pointer to the RendParams object.
 */
QtSceneDrawer::RendParams* QtSceneDrawer::get_rend_params() {
  return rend_params_;
}

/**
 * @brief Loads the current model view settings.
 * If there are no saved settings, the default settings are used.
 */
void QtSceneDrawer::load_view_settings() {
  rend_params_->projection =
      view_settings_.value("projection", RendParams::Projection::PARALLEL)
          .toInt();
  rend_params_->edge_type = static_cast<RendParams::EdgesType>(
      view_settings_.value("edge_type", RendParams::EdgesType::SOLID).toInt());
  rend_params_->vertex_type = static_cast<RendParams::VerticesType>(
      view_settings_.value("vertex_type", RendParams::VerticesType::NO_VERTICES)
          .toInt());
  rend_params_->bg_color =
      view_settings_.value("bg_color", QColor(184, 213, 238)).toString();
  rend_params_->edge_color =
      view_settings_.value("edge_color", QColor(0, 0, 255)).toString();
  rend_params_->vertex_color =
      view_settings_.value("vertex_color", QColor(0, 0, 0)).toString();
  rend_params_->edge_thickness =
      view_settings_.value("edge_thickness", 1).toInt();
  rend_params_->vertex_size = view_settings_.value("vertex_size", 3).toInt();
}

/**
 * @brief Saves the current model view settings.
 *
 * By default the settings are saved in
 * /home/user/.config/company_name/project_name.conf in Ubuntu
 */
void QtSceneDrawer::save_view_settings() {
  view_settings_.setValue("projection", rend_params_->projection);
  view_settings_.setValue("edge_type", rend_params_->edge_type);
  view_settings_.setValue("vertex_type", rend_params_->vertex_type);
  view_settings_.setValue("bg_color", rend_params_->bg_color);
  view_settings_.setValue("edge_color", rend_params_->edge_color);
  view_settings_.setValue("vertex_color", rend_params_->vertex_color);
  view_settings_.setValue("edge_thickness", rend_params_->edge_thickness);
  view_settings_.setValue("vertex_size", rend_params_->vertex_size);
}

/**
 * @brief Fills array of points for drawing 3D model from Figure object.
 *
 * If the figure has edges, the function generates an array of 6 floats per
 * edge. If the figure has no edges, the function generates an array of 3 floats
 * per vertex.
 *
 * @param figure The Figure object to be drawn.
 */
void QtSceneDrawer::DrawFigure(Figure* figure) {
  if (edges_array_ != nullptr) {
    delete[] edges_array_;
    edges_array_ = nullptr;
  }

  size_t i = 0;
  if (figure->get_edges().size() != 0) {
    array_size_ = figure->get_edges().size() * 6;
    edges_array_ = new float[array_size_];

    for (Edge& edge : figure->get_edges()) {
      const Point& begin = edge.get_begin().get_position();
      const Point& end = edge.get_end().get_position();
      edges_array_[i++] = begin.x_;
      edges_array_[i++] = begin.y_;
      edges_array_[i++] = begin.z_;
      edges_array_[i++] = end.x_;
      edges_array_[i++] = end.y_;
      edges_array_[i++] = end.z_;
    }
  } else {
    array_size_ = figure->get_vertices().size() * 3;
    edges_array_ = new float[array_size_];

    for (auto& vertex : figure->get_vertices()) {
      edges_array_[i++] = vertex.get_position().x_;
      edges_array_[i++] = vertex.get_position().y_;
      edges_array_[i++] = vertex.get_position().z_;
    }
  }
}
}  // namespace s21
