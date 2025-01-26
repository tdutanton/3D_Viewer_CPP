#ifndef QT_SCENE_DRAWER_H
#define QT_SCENE_DRAWER_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QWidget>
#include <QtOpenGL/QtOpenGL>
#include <QtOpenGL>

#include "../../model/facade/scene_drawer_base.h"
#include "../../model/figure/figure.h"

#define DRAWER_WIDTH 1000
#define DRAWER_HEIGHT 800
#define DRAWER_INDENT 5

namespace s21 {

/**
 * @class QtSceneDrawer
 * @brief The QtSceneDrawer is an OpenGL-based widget for rendering 3D figures.
 */
class QtSceneDrawer : public QOpenGLWidget,
                      protected QOpenGLFunctions,
                      public SceneDrawerBase {
  Q_OBJECT
 public:
  /**
   * @struct RendParams
   * @brief Struct contains rendering parameters for the 3D scene (projection
   * type, background color, edge and vertex rendering styles and colors).
   */
  struct RendParams {
    /**
     * @enum Projection
     * @brief Projection type
     */
    enum Projection { PARALLEL = 0, CENTRAL };
    /**
     * @enum EdgesType
     * @brief Edge rendering style
     */
    enum EdgesType { NO_EDGES = 0, SOLID, DASHED };
    /**
     * @enum VerticesType
     * @brief Vertex rendering style
     */
    enum VerticesType { NO_VERTICES = 0, CIRCLE, SQUARE };
    // Projection type
    int projection = PARALLEL;
    // Background color (RGB 0 - 255)
    QColor bg_color = {184, 213, 238};
    // Edge rendering style
    EdgesType edge_type = SOLID;
    int edge_thickness = 1;
    // Edge color (RGB 0 - 255)
    QColor edge_color = {0, 0, 255};
    // Vertex rendering style
    VerticesType vertex_type = NO_VERTICES;
    int vertex_size = 3;
    // Vertex color (RGB 0 - 255)
    QColor vertex_color = {0, 0, 0};
  };

  explicit QtSceneDrawer(QWidget* parent = Q_NULLPTR);
  ~QtSceneDrawer();

  // OpenGL functions

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  // other functions

  void DrawFigure(Figure* figure) override;
  RendParams* get_rend_params();
  void load_view_settings();
  void save_view_settings();

 protected:
  // array of points for drawing figure
  float* edges_array_;
  // edges_array size
  size_t array_size_;
  // rendering parameters
  RendParams* rend_params_;
  // settings for saving/loading view parameters
  QSettings view_settings_;
};
}  // namespace s21

#endif  // QT_SCENE_DRAWER_H
