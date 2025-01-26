#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QWidget>

#include "../model/facade/facade.h"
#include "controller/controller.h"
#include "view/view.h"

#define MAIN_WINDOW_WIDTH 1400
#define MAIN_WINDOW_HEIGHT 830

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow* ui;
  Facade* facade_;
  Controller* controller_;
  View* view_;

 private slots:
};
}  // namespace s21
#endif  // MAINWINDOW_H
