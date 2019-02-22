#ifndef COLORBUTTON_HH
#define COLORBUTTON_HH

#include <QToolButton>

class ColorButton : public QToolButton
{
  Q_OBJECT

public:
  ColorButton(const QColor &color, QWidget *parent=nullptr);

  const QColor &color() const;
  void setColor(const QColor &color);

protected slots:
  void onClicked();

protected:
  QColor _color;
};

#endif // COLORBUTTON_HH
