#include "mainwindow.h"

#include "ui_mainwindow.h"

namespace s21 {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setWindowTitle("3DViewer");
  this->setWindowIcon(QIcon(":/img/images/icon.png"));
  this->setFixedSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);

  facade_ = new Facade();
  controller_ = new Controller(facade_);
  view_ = new View(this, controller_);

  setCentralWidget(view_);
  setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow() {
  delete ui;
  delete facade_;
  delete controller_;
}
}  // namespace s21
