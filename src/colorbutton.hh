#ifndef COLORBUTTON_HH
#define COLORBUTTON_HH

#include <QPushButton>

class ColorButton : public QPushButton
{
  Q_OBJECT

public:
  ColorButton(const QColor &color, QWidget *parent=nullptr);

  QColor color();
  void setColor(const QColor &color);

protected slots:
  void onClicked();
};

#endif // COLORBUTTON_HH
