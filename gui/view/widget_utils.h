#ifndef WIDGETUTILS_H
#define WIDGETUTILS_H

#include <QtWidgets>

namespace s21 {

QPushButton *setup_button(QWidget *parent, const QString &text,
                          const QString &icon_path, const QSize &icon_size,
                          int x, int y, int width, int height,
                          const QFont &font);
QLabel *setup_label(QWidget *parent, const QString &text, int x, int y,
                    int width, int height, const QFont &font, Qt::Alignment);
QScrollBar *setup_scrollbar(QWidget *parent, Qt::Orientation orient, int x,
                            int y, int width, int height, int min, int max);
QSpinBox *setup_spinbox(QWidget *parent, int x, int y, int width, int height,
                        int min, int max);
QComboBox *setup_combobox(QWidget *parent, const QStringList &options, int x,
                          int y, int width, int height, const QFont &font);
};  // namespace s21

#endif  // WIDGETUTILS_H
