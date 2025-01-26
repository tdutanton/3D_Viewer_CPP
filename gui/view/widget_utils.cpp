#include "widget_utils.h"

namespace s21 {

/**
 * @brief Creates a new QPushButton with the specified text, icon,
 * geometry, and font. If an icon path is provided, the button will display the
 * icon with the given size.
 *
 * @param parent The parent widget for the button.
 * @param text The text to be displayed on the button.
 * @param icon_path The path to the icon image file, or an empty string if no
 * icon is desired.
 * @param icon_size The size of the icon to be displayed on the button.
 * @param x The x-coordinate of the button's position.
 * @param y The y-coordinate of the button's position.
 * @param width The width of the button.
 * @param height The height of the button.
 * @param font The font to be used for the button's text.
 * @return A pointer to the newly created QPushButton.
 */
QPushButton *setup_button(QWidget *parent, const QString &text,
                          const QString &icon_path, const QSize &icon_size,
                          int x, int y, int width, int height,
                          const QFont &font) {
  QPushButton *button = new QPushButton(text, parent);
  button->setGeometry(x, y, width, height);
  button->setFont(font);
  if (icon_path != "") {
    button->setIcon(QIcon(icon_path));
    button->setIconSize(icon_size);
  }
  return button;
}

/**
 * @brief Creates a new QLabel with the specified text, geometry, font,
 * and alignment.
 *
 * @param parent The parent widget for the label.
 * @param text The text to be displayed on the label.
 * @param x The x-coordinate of the label's position.
 * @param y The y-coordinate of the label's position.
 * @param width The width of the label.
 * @param height The height of the label.
 * @param font The font to be used for the label's text.
 * @param align The alignment of the label's text.
 * @return A pointer to the newly created QLabel.
 */
QLabel *setup_label(QWidget *parent, const QString &text, int x, int y,
                    int width, int height, const QFont &font,
                    Qt::Alignment align) {
  QLabel *label = new QLabel(text, parent);
  label->setGeometry(x, y, width, height);
  label->setFont(font);
  label->setAlignment(align);
  return label;
}

/**
 * @brief Creates a new QScrollBar with the specified orientation, geometry, and
 * range.
 *
 * @param parent The parent widget for the scroll bar.
 * @param orient The orientation of the scroll bar (Qt::Horizontal or
 * Qt::Vertical).
 * @param x The x-coordinate of the scroll bar's position.
 * @param y The y-coordinate of the scroll bar's position.
 * @param width The width of the scroll bar.
 * @param height The height of the scroll bar.
 * @param min The minimum value of the scroll bar's range.
 * @param max The maximum value of the scroll bar's range.
 * @return A pointer to the newly created QScrollBar.
 */
QScrollBar *setup_scrollbar(QWidget *parent, Qt::Orientation orient, int x,
                            int y, int width, int height, int min, int max) {
  QScrollBar *scroll_bar = new QScrollBar(orient, parent);
  scroll_bar->setGeometry(x, y, width, height);
  scroll_bar->setRange(min, max);
  return scroll_bar;
}

/**
 * @brief Creates a new QSpinBox with the specified geometry and range.
 *
 * @param parent The parent widget for the spin box.
 * @param x The x-coordinate of the spin box's position.
 * @param y The y-coordinate of the spin box's position.
 * @param width The width of the spin box.
 * @param height The height of the spin box.
 * @param min The minimum value of the spin box's range.
 * @param max The maximum value of the spin box's range.
 * @return A pointer to the newly created QSpinBox.
 */
QSpinBox *setup_spinbox(QWidget *parent, int x, int y, int width, int height,
                        int min, int max) {
  QSpinBox *spinbox = new QSpinBox(parent);
  spinbox->setGeometry(x, y, width, height);
  spinbox->setRange(min, max);
  spinbox->setButtonSymbols(QSpinBox::PlusMinus);
  return spinbox;
}

/**
 * @brief Creates a new QComboBox with the specified parent, options, geometry,
 * and font.
 *
 * @param parent The parent widget for the combo box.
 * @param options The list of options to populate the combo box.
 * @param x The x-coordinate of the combo box's position.
 * @param y The y-coordinate of the combo box's position.
 * @param width The width of the combo box.
 * @param height The height of the combo box.
 * @param font The font to use for the combo box's text.
 * @return A pointer to the newly created QComboBox.
 */
QComboBox *setup_combobox(QWidget *parent, const QStringList &options, int x,
                          int y, int width, int height, const QFont &font) {
  QComboBox *combobox = new QComboBox(parent);
  combobox->setGeometry(x, y, width, height);
  combobox->addItems(options);
  combobox->setFont(font);
  combobox->setEditable(false);
  return combobox;
}

};  // namespace s21
